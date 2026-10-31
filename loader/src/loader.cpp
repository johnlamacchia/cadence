#include "loader.h"
#include "hook.h"
#include "crash.h"
#include <cadence_abi.h>
#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <intrin.h>
#include <psapi.h>

// Minimal JSON value reader (no dependencies)
namespace json_reader {
    struct Parser {
        const char* p;
        const char* end;

        void skip_ws() {
            while (p < end && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
        }

        std::string read_string() {
            skip_ws();
            if (*p != '"') return "";
            p++;
            std::string s;
            while (p < end && *p != '"') {
                if (*p == '\\' && p + 1 < end) p++;
                s += *p++;
            }
            if (p < end) p++; // skip closing "
            return s;
        }

        void skip_value() {
            skip_ws();
            if (*p == '"') { read_string(); return; }
            if (*p == '{') {
                p++; int depth = 1;
                while (p < end && depth > 0) {
                    if (*p == '{') depth++;
                    else if (*p == '}') depth--;
                    p++;
                }
                return;
            }
            if (*p == '[') {
                p++; int depth = 1;
                while (p < end && depth > 0) {
                    if (*p == '[') depth++;
                    else if (*p == ']') depth--;
                    p++;
                }
                return;
            }
            while (p < end && *p != ',' && *p != '}' && *p != ']') p++;
        }

        // Parse {"key": "value", ...} flat string→string map
        std::unordered_map<std::string, std::string> read_string_map() {
            std::unordered_map<std::string, std::string> m;
            skip_ws();
            if (*p != '{') return m;
            p++;
            while (true) {
                skip_ws();
                if (p >= end || *p == '}') { p++; break; }
                if (*p == ',') { p++; continue; }
                auto key = read_string();
                skip_ws();
                if (*p == ':') p++;
                skip_ws();
                if (*p == '"') {
                    m[key] = read_string();
                } else if (*p == '{') {
                    // Nested object: if key is "addresses" or "hashes", parse it
                    if (key == "addresses" || key == "hashes") {
                        auto inner = read_string_map();
                        for (auto& [k, v] : inner) m[k] = v;
                    } else {
                        skip_value();
                    }
                } else {
                    // numbers, bools etc: read as raw string
                    const char* start = p;
                    skip_value();
                    m[key] = std::string(start, p);
                }
            }
            return m;
        }
    };

    std::unordered_map<std::string, std::string> parse_file(const std::string& path) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return {};
        std::string data((std::istreambuf_iterator<char>(f)), {});
        Parser p{data.c_str(), data.c_str() + data.size()};
        return p.read_string_map();
    }
}

namespace loader {

static FILE* g_log = nullptr;
static std::unordered_map<std::string, void*> g_addresses;
static std::unordered_map<std::string, std::string> g_layout_hashes;
static std::string g_gd_version;

static void log(const char* fmt, ...) {
    if (!g_log) return;
    va_list ap;
    va_start(ap, fmt);
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fprintf(g_log, "\n");
    fflush(g_log);
}

// --- LoaderAPI implementation ---

static void* api_addr(const LoaderAPI*, const char* sig) {
    auto it = g_addresses.find(sig);
    if (it != g_addresses.end()) return it->second;
    log("[WARN] address not found: %s", sig);
    return nullptr;
}

static bool api_hook(const LoaderAPI*, void* target, void* detour, void** original) {
    return hook::install(target, detour, original);
}

// Removes the calling mod's hooks on `target` (other mods' hooks are untouched).
// The caller is identified by the return address, so the mod API stays unchanged.
__declspec(noinline) static bool api_unhook(const LoaderAPI*, void* target) {
    HMODULE caller = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCWSTR)_ReturnAddress(), &caller))
        return false;
    MODULEINFO mi;
    if (!GetModuleInformation(GetCurrentProcess(), caller, &mi, sizeof(mi))) return false;
    uintptr_t lo = (uintptr_t)mi.lpBaseOfDll;
    return hook::remove_in_range(target, lo, lo + mi.SizeOfImage) > 0;
}

static const char* api_gd_version(const LoaderAPI*) {
    return g_gd_version.c_str();
}

static void api_log(const LoaderAPI*, const char* fmt, ...) {
    if (!g_log) return;
    va_list ap;
    va_start(ap, fmt);
    fprintf(g_log, "[MOD] ");
    vfprintf(g_log, fmt, ap);
    va_end(ap);
    fprintf(g_log, "\n");
    fflush(g_log);
}

static std::vector<ModEntry> g_mod_entries;

static const ModEntry* api_get_mods(const LoaderAPI*, int* out_count) {
    *out_count = (int)g_mod_entries.size();
    return g_mod_entries.empty() ? nullptr : g_mod_entries.data();
}

static LoaderAPI g_api = {api_addr, api_hook, api_unhook, api_gd_version, api_log, api_get_mods};

// --- Address loading ---

// True if `rva` is inside one of GeometryDash.exe's executable sections. Anything
// else (a placeholder, a typo in a local override, an address from another GD
// version) would be hooked as garbage, so it is refused here instead.
static bool rva_is_code(uintptr_t rva) {
    auto base = (const uint8_t*)GetModuleHandleW(nullptr);
    auto nt = (const IMAGE_NT_HEADERS64*)(base + ((const IMAGE_DOS_HEADER*)base)->e_lfanew);
    auto sec = IMAGE_FIRST_SECTION(nt);
    for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if ((sec[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) &&
            rva >= sec[i].VirtualAddress && rva < sec[i].VirtualAddress + sec[i].Misc.VirtualSize)
            return true;
    }
    return false;
}

// Adds every "signature": "0xRVA" pair of `data`; returns how many were accepted.
static int add_addresses(const std::unordered_map<std::string, std::string>& data, const char* what) {
    uintptr_t image_base = (uintptr_t)GetModuleHandleW(nullptr);
    int accepted = 0, rejected = 0;
    std::string example;
    for (auto& [sig, addr_str] : data) {
        if (!(addr_str.starts_with("0x") || addr_str.starts_with("0X"))) continue;   // version, count, lists...
        uintptr_t rva = strtoull(addr_str.c_str(), nullptr, 16);
        if (!rva_is_code(rva)) {
            if (rejected++ == 0) example = sig + " = " + addr_str;
            continue;
        }
        g_addresses[sig] = (void*)(image_base + rva);
        accepted++;
    }
    if (rejected)
        log("[WARN] %s: ignored %d address(es) that are not inside GeometryDash.exe's code, e.g. %s",
            what, rejected, example.c_str());
    return accepted;
}

static bool load_addresses(const std::filesystem::path& base_dir) {
    for (auto& entry : std::filesystem::directory_iterator(base_dir)) {
        auto name = entry.path().filename().string();
        // addresses_rejected_<ver>.json is a report of addresses dropped at generation time, not an address file.
        if (name.starts_with("addresses_") && name.ends_with(".json") && !name.starts_with("addresses_rejected_")) {
            g_gd_version = name.substr(10, name.size() - 15); // strip "addresses_" and ".json"
            int n = add_addresses(json_reader::parse_file(entry.path().string()), name.c_str());
            log("[INFO] loaded %d addresses for GD %s", n, g_gd_version.c_str());
            break;
        }
    }

    // Local overrides win over the official file.
    auto local_path = base_dir / "addresses.local.json";
    if (std::filesystem::exists(local_path)) {
        int n = add_addresses(json_reader::parse_file(local_path.string()), "addresses.local.json");
        if (n > 0) log("[INFO] loaded %d local address overrides", n);
    }

    return !g_addresses.empty();
}

static bool load_layout_hashes(const std::filesystem::path& base_dir) {
    for (auto& entry : std::filesystem::directory_iterator(base_dir)) {
        auto name = entry.path().filename().string();
        if (name.starts_with("layout_hashes_") && name.ends_with(".json")) {
            g_layout_hashes = json_reader::parse_file(entry.path().string());
            log("[INFO] loaded %zu layout hashes", g_layout_hashes.size());
            return true;
        }
    }
    return false;
}

// --- Mod loading ---

using ModMainFunc = void (*)(const LoaderAPI* api);
using ModUnloadFunc = void (*)();
using ModInfoFunc = const char* (*)();
using ModHashesFunc = const char* (*)();

struct ModInfoInternal {
    std::string filename;
    HMODULE handle = nullptr;
    std::string display_name;
    std::string author;
    std::string description;
    std::string keybind;
    std::string version;
    bool enabled = true;
};

static std::vector<ModInfoInternal> g_loaded_mods;
static std::filesystem::path g_base_dir;

static void rebuild_mod_entries() {
    g_mod_entries.clear();
    g_mod_entries.reserve(g_loaded_mods.size());
    for (auto& m : g_loaded_mods) {
        ModEntry e;
        e.filename    = m.filename.c_str();
        e.name        = m.display_name.empty() ? m.filename.c_str() : m.display_name.c_str();
        e.author      = m.author.c_str();
        e.description = m.description.c_str();
        e.keybind     = m.keybind.c_str();
        e.version     = m.version.c_str();
        e.enabled     = m.enabled;
        g_mod_entries.push_back(e);
    }
}

static std::string json_get(const std::unordered_map<std::string, std::string>& m, const char* key) {
    auto it = m.find(key);
    return it != m.end() ? it->second : "";
}

static void parse_mod_info(ModInfoInternal& mod) {
    auto get_info = reinterpret_cast<ModInfoFunc>(GetProcAddress(mod.handle, "mod_info"));
    if (!get_info) return;
    const char* raw = get_info();
    if (!raw) return;
    json_reader::Parser p{raw, raw + strlen(raw)};
    auto data = p.read_string_map();
    mod.display_name = json_get(data, "name");
    mod.author       = json_get(data, "author");
    mod.description  = json_get(data, "desc");
    mod.keybind      = json_get(data, "key");
    mod.version      = json_get(data, "ver");
}

static bool check_mod_hashes(HMODULE mod, const std::string& mod_name) {
    // Mods export mod_layout_hashes(): a JSON object of class name -> layout hash.
    auto get_hashes = reinterpret_cast<ModHashesFunc>(GetProcAddress(mod, "mod_layout_hashes"));
    if (!get_hashes) return true;   // no hash info, nothing to check

    const std::string raw = get_hashes();
    json_reader::Parser p{raw.c_str(), raw.c_str() + raw.size()};
    auto mod_hashes = p.read_string_map();

    for (auto& [cls, hash] : mod_hashes) {
        auto it = g_layout_hashes.find(cls);
        if (it != g_layout_hashes.end() && it->second != hash) {
            log("[ERROR] mod '%s' was built against class '%s' (layout hash %s) but this GD has %s - refusing to load it",
                mod_name.c_str(), cls.c_str(), hash.c_str(), it->second.c_str());
            return false;
        }
    }
    return true;
}

// Mods are loaded from shadow copies in mods/.cadence-cache/, so the originals
// in mods/ are never locked and can be rebuilt while the game is running.
static std::filesystem::path g_cache_dir;
static int g_generation = 0;

// Hot reload runs inside the drawScene hook chain, so a retired mod's own
// detours can still be on the call stack, and UI it created (button callbacks,
// scheduled selectors) can still point into its code. Unmapping it would turn
// either into a crash. So a retired mod gets mod_unload() and has every hook
// removed, but its DLL stays mapped for the rest of the session (a few KB).
static void unload_all_mods() {
    for (auto it = g_loaded_mods.rbegin(); it != g_loaded_mods.rend(); ++it) {
        auto unload = reinterpret_cast<ModUnloadFunc>(GetProcAddress(it->handle, "mod_unload"));
        if (unload) {
            log("[INFO] calling mod_unload for '%s'", it->filename.c_str());
            unload();
        }
        MODULEINFO mi;
        if (GetModuleInformation(GetCurrentProcess(), it->handle, &mi, sizeof(mi))) {
            int removed = hook::remove_by_detour_range(
                (uintptr_t)mi.lpBaseOfDll,
                (uintptr_t)mi.lpBaseOfDll + mi.SizeOfImage);
            if (removed > 0)
                log("[INFO] removed %d hooks from '%s'", removed, it->filename.c_str());
        }
        log("[INFO] retired '%s'", it->filename.c_str());
    }
    g_loaded_mods.clear();
    g_mod_entries.clear();
}

static void load_mods(const std::filesystem::path& mods_dir) {
    if (!std::filesystem::exists(mods_dir)) {
        log("[INFO] no mods/ directory, nothing to load");
        return;
    }

    std::vector<std::filesystem::path> dlls;
    for (auto& entry : std::filesystem::directory_iterator(mods_dir)) {
        if (entry.path().extension() == ".dll") {
            dlls.push_back(entry.path());
        }
    }
    std::sort(dlls.begin(), dlls.end());

    std::error_code ec;
    std::filesystem::create_directories(g_cache_dir, ec);
    g_generation++;

    for (auto& dll_path : dlls) {
        auto mod_name = dll_path.filename().string();

        auto shadow = g_cache_dir / (dll_path.stem().wstring() + L"." + std::to_wstring(g_generation) + L".dll");
        std::filesystem::copy_file(dll_path, shadow, std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) {
            log("[WARN] could not shadow-copy '%s' (%s), loading in place", mod_name.c_str(), ec.message().c_str());
            shadow = dll_path;
        }

        HMODULE mod = LoadLibraryW(shadow.c_str());
        if (!mod) {
            log("[ERROR] failed to load mod '%s': error %lu", mod_name.c_str(), GetLastError());
            continue;
        }

        if (!check_mod_hashes(mod, mod_name)) {
            FreeLibrary(mod);
            continue;
        }

        auto mod_main = reinterpret_cast<ModMainFunc>(GetProcAddress(mod, "mod_main"));
        if (!mod_main) {
            log("[WARN] mod '%s' has no mod_main export, skipping", mod_name.c_str());
            FreeLibrary(mod);
            continue;
        }

        log("[INFO] loading mod '%s'", mod_name.c_str());
        mod_main(&g_api);

        ModInfoInternal info;
        info.filename = mod_name;
        info.handle = mod;
        parse_mod_info(info);
        g_loaded_mods.push_back(std::move(info));
        log("[INFO] mod '%s' initialized", mod_name.c_str());
    }
    rebuild_mod_entries();
}

// --- Main-thread reload via drawScene hook ---

static std::atomic<bool> g_reload_pending{false};
static void(__fastcall* orig_drawScene)(void*) = nullptr;

static void show_reload_toast(void* director, int mod_count) {
    using getRunningScene_t = void* (__fastcall*)(void*);
    using addChild_t = void(__fastcall*)(void*, void*, int);

    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return;

    auto getRunningScene = reinterpret_cast<getRunningScene_t>(GetProcAddress(cocos,
        "?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ"));
    auto addChild = reinterpret_cast<addChild_t>(GetProcAddress(cocos,
        "?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z"));
    if (!getRunningScene || !addChild) return;

    void* scene = getRunningScene(director);
    if (!scene) return;

    using TextAlertPopup_create_t = void* (*)(std::string*, float, float, int, std::string*);
    auto textAlertCreate = reinterpret_cast<TextAlertPopup_create_t>(api_addr(nullptr,
        "TextAlertPopup::create(std::string text, float delay, float scale, int opacity, std::string font)"));
    if (!textAlertCreate) return;

    char msg[128];
    snprintf(msg, sizeof(msg), "Cadence: reloaded %d mod%s", mod_count, mod_count == 1 ? "" : "s");
    std::string text(msg);
    std::string font("bigFont.fnt");
    void* popup = textAlertCreate(&text, 1.5f, 0.6f, 200, &font);
    if (popup) {
        addChild(scene, popup, 10000);
        log("[INFO] reload toast shown");
    }
}

static void __fastcall hook_drawScene(void* director) {
    if (g_reload_pending.exchange(false)) {
        log("[INFO] === hot reload triggered ===");
        unload_all_mods();
        load_mods(g_base_dir / "mods");
        int count = (int)g_loaded_mods.size();
        log("[INFO] === hot reload complete ===");
        show_reload_toast(director, count);
    }
    orig_drawScene(director);
}

void reload_mods() {
    g_reload_pending = true;
}

// Polls F6 for the life of the process; it only sets a flag, the reload itself
// happens on the render thread in hook_drawScene.
static DWORD WINAPI hotkey_thread(LPVOID) {
    bool was_down = false;
    for (;;) {
        bool is_down = (GetAsyncKeyState(VK_F6) & 0x8000) != 0;
        if (is_down && !was_down) {
            g_reload_pending = true;
            Sleep(500);
        }
        was_down = is_down;
        Sleep(30);
    }
}

static bool install_drawscene_hook() {
    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return false;
    void* drawScene = reinterpret_cast<void*>(GetProcAddress(cocos,
        "?drawScene@CCDirector@cocos2d@@QEAAXXZ"));
    if (!drawScene) return false;
    return hook::install(drawScene, reinterpret_cast<void*>(hook_drawScene),
                         reinterpret_cast<void**>(&orig_drawScene));
}

static std::string utf8_path(const std::filesystem::path& p) {
    auto u = p.u8string();
    return std::string(reinterpret_cast<const char*>(u.data()), u.size());
}

// Wine (and so Proton) exports wine_get_version from ntdll; Windows does not.
static void log_environment() {
    if (HMODULE nt = GetModuleHandleW(L"ntdll.dll"))
        if (auto wine_version = reinterpret_cast<const char* (__cdecl*)()>(GetProcAddress(nt, "wine_get_version")))
            log("[INFO] running under Wine %s", wine_version());
}

bool init() {
    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_path, MAX_PATH);
    g_base_dir = std::filesystem::path(exe_path).parent_path();

    auto log_path = g_base_dir / "loader.log";
    g_log = _wfopen(log_path.c_str(), L"w");
    log("[INFO] Cadence loader starting");
    log("[INFO] game folder: %s", utf8_path(g_base_dir).c_str());
    log_environment();

    crash::init();
    log("[INFO] crash handler installed");

    g_cache_dir = g_base_dir / "mods" / ".cadence-cache";
    {
        // Clear shadow copies from previous sessions (locked ones just stay).
        std::error_code ec;
        for (auto& e : std::filesystem::directory_iterator(g_cache_dir, ec))
            std::filesystem::remove(e.path(), ec);
    }

    auto gen_dir = g_base_dir / "generated";
    if (!std::filesystem::exists(gen_dir)) gen_dir = g_base_dir;

    if (!load_addresses(gen_dir)) {
        log("[ERROR] no usable addresses in %s: addresses_<version>.json is missing, or none of its addresses "
            "fit this GeometryDash.exe. Mods cannot resolve GD functions by name (hooks on cocos2d exports still "
            "work). Run the installer, or copy the release's generated/ folder next to GeometryDash.exe.",
            utf8_path(gen_dir).c_str());
    }
    load_layout_hashes(gen_dir);

    load_mods(g_base_dir / "mods");

    if (install_drawscene_hook()) {
        log("[INFO] drawScene hook installed for main-thread reload");
    }
    if (HANDLE t = CreateThread(nullptr, 0, hotkey_thread, nullptr, 0, nullptr))
        CloseHandle(t);
    log("[INFO] hot reload ready (press F6)");

    log("[INFO] loader init complete");
    return true;
}

}
