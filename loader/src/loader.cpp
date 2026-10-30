#include "loader.h"
#include "hook.h"
#include "crash.h"
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <fstream>
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
                if (*p == '\\') { p++; s += *p++; }
                else s += *p++;
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
                    // numbers, bools etc — read as raw string
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

struct LoaderAPI {
    void* (*addr)(const LoaderAPI* self, const char* signature);
    bool  (*hook_fn)(const LoaderAPI* self, void* target, void* detour, void** original);
    bool  (*unhook_fn)(const LoaderAPI* self, void* target);
    const char* (*gd_version_fn)(const LoaderAPI* self);
};

static void* api_addr(const LoaderAPI*, const char* sig) {
    auto it = g_addresses.find(sig);
    if (it != g_addresses.end()) return it->second;
    log("[WARN] address not found: %s", sig);
    return nullptr;
}

static bool api_hook(const LoaderAPI*, void* target, void* detour, void** original) {
    return hook::install(target, detour, original);
}

static bool api_unhook(const LoaderAPI*, void* target) {
    return hook::remove(target);
}

static const char* api_gd_version(const LoaderAPI*) {
    return g_gd_version.c_str();
}

static LoaderAPI g_api = {api_addr, api_hook, api_unhook, api_gd_version};

// --- Address loading ---

static bool load_addresses(const std::filesystem::path& base_dir) {
    // Find addresses JSON
    for (auto& entry : std::filesystem::directory_iterator(base_dir)) {
        auto name = entry.path().filename().string();
        if (name.starts_with("addresses_") && name.ends_with(".json")) {
            g_gd_version = name.substr(10, name.size() - 15); // strip "addresses_" and ".json"

            auto data = json_reader::parse_file(entry.path().string());
            uintptr_t image_base = (uintptr_t)GetModuleHandleW(nullptr);
            int count = 0;
            for (auto& [sig, addr_str] : data) {
                if (sig == "version" || sig == "count") continue;
                if (addr_str.starts_with("0x") || addr_str.starts_with("0X")) {
                    uintptr_t rva = strtoull(addr_str.c_str(), nullptr, 16);
                    g_addresses[sig] = (void*)(image_base + rva);
                    count++;
                }
            }
            log("[INFO] loaded %d addresses for GD %s", count, g_gd_version.c_str());
            break;
        }
    }

    // Load local overrides
    auto local_path = base_dir / "addresses.local.json";
    if (std::filesystem::exists(local_path)) {
        auto data = json_reader::parse_file(local_path.string());
        uintptr_t image_base = (uintptr_t)GetModuleHandleW(nullptr);
        int count = 0;
        for (auto& [sig, addr_str] : data) {
            if (addr_str.starts_with("0x") || addr_str.starts_with("0X")) {
                uintptr_t rva = strtoull(addr_str.c_str(), nullptr, 16);
                g_addresses[sig] = (void*)(image_base + rva);
                count++;
            }
        }
        if (count > 0) log("[INFO] loaded %d local address overrides", count);
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

typedef void (*ModMainFunc)(const LoaderAPI* api);
typedef void (*ModUnloadFunc)();

struct ModInfo {
    std::filesystem::path path;
    std::string name;
    HMODULE handle;
};

static std::vector<ModInfo> g_loaded_mods;
static std::filesystem::path g_base_dir;
static CRITICAL_SECTION g_reload_lock;
static bool g_reload_lock_init = false;

static bool check_mod_hashes(HMODULE mod, const std::string& mod_name) {
    // Mod exports: const char* mod_layout_hashes() → JSON string of class→hash
    auto get_hashes = (const char* (*)())GetProcAddress(mod, "mod_layout_hashes");
    if (!get_hashes) return true; // no hash info = skip check

    std::string raw = get_hashes();
    json_reader::Parser p{raw.c_str(), raw.c_str() + raw.size()};
    auto mod_hashes = p.read_string_map();

    for (auto& [cls, hash] : mod_hashes) {
        auto it = g_layout_hashes.find(cls);
        if (it != g_layout_hashes.end() && it->second != hash) {
            log("[ERROR] mod '%s' uses class '%s' with layout hash %s, but current is %s — refusing to load",
                mod_name.c_str(), cls.c_str(), hash.c_str(), it->second.c_str());
            return false;
        }
    }
    return true;
}

static void unload_all_mods() {
    for (auto it = g_loaded_mods.rbegin(); it != g_loaded_mods.rend(); ++it) {
        auto unload = (ModUnloadFunc)GetProcAddress(it->handle, "mod_unload");
        if (unload) {
            log("[INFO] calling mod_unload for '%s'", it->name.c_str());
            unload();
        }
        MODULEINFO mi;
        if (GetModuleInformation(GetCurrentProcess(), it->handle, &mi, sizeof(mi))) {
            int removed = hook::remove_by_detour_range(
                (uintptr_t)mi.lpBaseOfDll,
                (uintptr_t)mi.lpBaseOfDll + mi.SizeOfImage);
            if (removed > 0)
                log("[INFO] removed %d hooks from '%s'", removed, it->name.c_str());
        }
        FreeLibrary(it->handle);
        log("[INFO] unloaded '%s'", it->name.c_str());
    }
    g_loaded_mods.clear();
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

    for (auto& dll_path : dlls) {
        auto mod_name = dll_path.filename().string();
        HMODULE mod = LoadLibraryW(dll_path.c_str());
        if (!mod) {
            log("[ERROR] failed to load mod '%s': error %lu", mod_name.c_str(), GetLastError());
            continue;
        }

        if (!check_mod_hashes(mod, mod_name)) {
            FreeLibrary(mod);
            continue;
        }

        auto mod_main = (ModMainFunc)GetProcAddress(mod, "mod_main");
        if (!mod_main) {
            log("[WARN] mod '%s' has no mod_main export, skipping", mod_name.c_str());
            FreeLibrary(mod);
            continue;
        }

        log("[INFO] loading mod '%s'", mod_name.c_str());
        mod_main(&g_api);
        g_loaded_mods.push_back({dll_path, mod_name, mod});
        log("[INFO] mod '%s' initialized", mod_name.c_str());
    }
}

static void show_reload_toast(int mod_count) {
    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return;

    using sharedDirector_t = void* (*)();
    using getRunningScene_t = void* (__fastcall*)(void*);
    using addChild_t = void(__fastcall*)(void*, void*, int);

    auto sharedDirector = (sharedDirector_t)GetProcAddress(cocos,
        "?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    auto getRunningScene = (getRunningScene_t)GetProcAddress(cocos,
        "?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    auto addChild = (addChild_t)GetProcAddress(cocos,
        "?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");

    if (!sharedDirector || !getRunningScene || !addChild) return;

    void* director = sharedDirector();
    if (!director) return;
    void* scene = getRunningScene(director);
    if (!scene) return;

    // TextAlertPopup::create(std::string text, float delay, float scale, int opacity, std::string font)
    uintptr_t image_base = (uintptr_t)GetModuleHandleW(nullptr);
    using TextAlertPopup_create_t = void* (*)(std::string*, float, float, int, std::string*);
    auto textAlertCreate = (TextAlertPopup_create_t)(image_base + 0x3a8760);

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

void reload_mods() {
    if (!g_reload_lock_init) return;
    EnterCriticalSection(&g_reload_lock);
    log("[INFO] === hot reload triggered ===");
    unload_all_mods();
    load_mods(g_base_dir / "mods");
    int count = (int)g_loaded_mods.size();
    log("[INFO] === hot reload complete ===");
    show_reload_toast(count);
    LeaveCriticalSection(&g_reload_lock);
}

static DWORD WINAPI hotkey_thread(LPVOID) {
    while (true) {
        if (GetAsyncKeyState(VK_F6) & 1) {
            reload_mods();
            Sleep(500);
        }
        Sleep(50);
    }
    return 0;
}

bool init() {
    InitializeCriticalSection(&g_reload_lock);
    g_reload_lock_init = true;

    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_path, MAX_PATH);
    g_base_dir = std::filesystem::path(exe_path).parent_path();

    auto log_path = g_base_dir / "loader.log";
    g_log = _wfopen(log_path.c_str(), L"w");
    log("[INFO] Cadence loader starting");

    crash::init();
    log("[INFO] crash handler installed");

    auto gen_dir = g_base_dir / "generated";
    if (!std::filesystem::exists(gen_dir)) gen_dir = g_base_dir;

    if (!load_addresses(gen_dir)) {
        log("[ERROR] no address file found — mods will not be able to resolve functions");
    }
    load_layout_hashes(gen_dir);

    load_mods(g_base_dir / "mods");

    CreateThread(nullptr, 0, hotkey_thread, nullptr, 0, nullptr);
    log("[INFO] hot reload ready (press F6)");

    log("[INFO] loader init complete");
    return true;
}

}
