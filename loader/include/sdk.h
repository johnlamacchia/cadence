// Cadence: the whole mod-facing API. Read time: ~5 minutes.
//
// A mod is a DLL in <GD>/mods/ that exports one function:
//
//     extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api);
//
// The loader does exactly three things for you:
//   1. addr    resolve a GD function's address from its signature
//   2. hook    install / remove an x64 hook (hooks from many mods chain)
//   3. load    call mod_main once per mod, in alphabetical order
// plus gd_version() and log() (a line in loader.log).
//
// Nothing is wrapped: you call the real GD functions with their real signatures.
// The typed layer at the bottom just checks your hook's signature at compile
// time against the generated headers in generated/include/gd/.

#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <cstdio>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// ── C API ─────────────────────────────────────────────────────────────

// One loaded mod, as reported by api->get_mods. Strings come from the metadata
// that cadence_add_mod() bakes into the DLL (see cmake/CadenceMod.cmake).
struct ModEntry {
    const char* filename;
    const char* name;
    const char* author;
    const char* description;
    const char* keybind;
    const char* version;
    bool enabled;
};

struct LoaderAPI {
    // Address of a GD function, by the exact signature string used as a key in
    // generated/addresses_<ver>.json (e.g. "PlayLayer::resetLevel()").
    // nullptr, plus a warning in loader.log, if there is no known address.
    void* (*addr)(const LoaderAPI* self, const char* signature);

    // Install an x64 hook on `target`. On success *original receives the next
    // function in the chain: the previously installed detour, or a trampoline to
    // the real code. Hooks from any number of mods on one function chain.
    bool (*hook_fn)(const LoaderAPI* self, void* target, void* detour, void** original);

    // Remove the calling mod's hooks on `target`. Safe in any order: hooks
    // installed after it are re-linked. You normally never need this; the loader
    // removes a mod's hooks when it is unloaded.
    bool (*unhook_fn)(const LoaderAPI* self, void* target);

    // "2.2091": the GD version the loaded address file was generated for.
    const char* (*gd_version_fn)(const LoaderAPI* self);

    // Append a printf-style line to loader.log.
    void (*log_fn)(const LoaderAPI* self, const char* fmt, ...);

    // The mods the loader has loaded. Valid until the next reload.
    const ModEntry* (*get_mods)(const LoaderAPI* self, int* out_count);
};

#define GD_ADDR(api, sig)                  ((api)->addr((api), (sig)))
#define GD_HOOK(api, target, detour, orig) ((api)->hook_fn((api), (target), (detour), (orig)))
#define GD_UNHOOK(api, target)             ((api)->unhook_fn((api), (target)))
#define GD_VERSION(api)                    ((api)->gd_version_fn((api)))

// ── C++ layer (header-only) ───────────────────────────────────────────

#ifdef __cplusplus
#include <string>
#include <type_traits>

namespace cadence {

inline const LoaderAPI* g_api = nullptr;

inline void set_api(const LoaderAPI* api) { g_api = api; }

inline void* addr(const char* sig) {
    return g_api ? g_api->addr(g_api, sig) : nullptr;
}

inline bool hook(void* target, void* detour, void** original) {
    return g_api && target ? g_api->hook_fn(g_api, target, detour, original) : false;
}

inline bool unhook(void* target) {
    return g_api ? g_api->unhook_fn(g_api, target) : false;
}

inline const char* gd_version() {
    return g_api ? g_api->gd_version_fn(g_api) : "unknown";
}

inline void log(const char* fmt, ...) {
    if (!g_api || !g_api->log_fn) return;
    va_list ap;
    va_start(ap, fmt);
    char buf[512];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    g_api->log_fn(g_api, "%s", buf);
}

inline const ModEntry* get_mods(int* count) {
    if (!g_api || !g_api->get_mods) { *count = 0; return nullptr; }
    return g_api->get_mods(g_api, count);
}

#ifdef _WIN32
// An export of libcocos2d.dll, by mangled name. This is just GetProcAddress.
inline void* cocos(const char* mangled_name) {
    static HMODULE m = GetModuleHandleA("libcocos2d.dll");
    return m ? (void*)GetProcAddress(m, mangled_name) : nullptr;
}
#endif

// ── Typed hooks ───────────────────────────────────────────────────────
//
//     #include <gd/PlayLayer.hpp>
//
//     static void __fastcall my_reset(PlayLayer* self) {
//         cadence::original<&PlayLayer::resetLevel>(self);   // call through
//     }
//     cadence::hook<&PlayLayer::resetLevel>(my_reset);
//
// The detour's type is derived from the member function, so a wrong signature
// is a compile error. Overloads need the usual cast:
//     cadence::hook<static_cast<bool (Foo::*)(int)>(&Foo::bar)>(detour);
// One hook per function per mod.

// Specialised by gd/<Class>.hpp for every member function with a Windows address.
template <auto F> inline constexpr const char* fn_sig = nullptr;

namespace detail {

// Whether MSVC returns R in a register (so the member-function and free-function
// calling conventions agree). Large or non-trivial structs use a hidden return
// pointer whose position differs between the two.
template <class R, bool Simple = std::is_void_v<R> || std::is_scalar_v<R>>
struct returns_in_register_impl : std::true_type {};
template <class R>
struct returns_in_register_impl<R, false> : std::bool_constant<
    std::is_trivially_copyable_v<R> && std::is_standard_layout_v<R> &&
    (sizeof(R) == 1 || sizeof(R) == 2 || sizeof(R) == 4 || sizeof(R) == 8)> {};
template <class R> inline constexpr bool returns_in_register = returns_in_register_impl<R>::value;

template <class T> struct fn_traits;
template <class R, class C, class... A> struct fn_traits<R (C::*)(A...)> {
    using ret = R; using type = R(__fastcall*)(C*, A...);
};
template <class R, class C, class... A> struct fn_traits<R (C::*)(A...) const> {
    using ret = R; using type = R(__fastcall*)(const C*, A...);
};
template <class R, class... A> struct fn_traits<R (*)(A...)> {
    using ret = R; using type = R(__fastcall*)(A...);
};

// Each class header registers the hash of the layout it was generated with.
// mod_layout_hashes() (below) reports them so the loader can refuse a mod whose
// classes changed, naming the class, instead of letting GD crash.
struct LayoutRegistration {
    const char* cls;
    const char* hash;
    LayoutRegistration* next;
    LayoutRegistration(const char* c, const char* h) : cls(c), hash(h), next(head()) { head() = this; }
    static LayoutRegistration*& head() { static LayoutRegistration* h = nullptr; return h; }
};

} // namespace detail

// The detour type for F: ret(__fastcall*)(Class* self, args...).
template <auto F> using fn_t = typename detail::fn_traits<decltype(F)>::type;

// The next function in F's hook chain, for calling through from your detour.
template <auto F> inline fn_t<F> original = nullptr;

template <auto F> bool hook(fn_t<F> detour) {
    static_assert(fn_sig<F> != nullptr,
        "no Windows address is known for this function: include its gd/<Class>.hpp, "
        "or it is marked `win inline` in the bindings (not hookable)");
    static_assert(detail::returns_in_register<typename detail::fn_traits<decltype(F)>::ret>,
        "this function returns a struct through a hidden pointer; hook it by hand with "
        "cadence::hook(addr, detour, &orig) and declare the detour with the hidden pointer first");
    void* target = addr(fn_sig<F>);
    if (!target) return false;
    return hook(target, (void*)detour, (void**)&original<F>);
}

// A typed pointer to the game's own implementation of F, to call it:
//     auto reset = cadence::fn<&PlayLayer::resetLevel>();   // void(__fastcall*)(PlayLayer*)
//     if (reset) reset(layer);
// nullptr if the address is unknown. For a virtual function this calls that
// class's implementation directly, without virtual dispatch.
template <auto F> fn_t<F> fn() {
    static_assert(fn_sig<F> != nullptr, "no Windows address is known for this function");
    return reinterpret_cast<fn_t<F>>(addr(fn_sig<F>));
}

// Untyped, string-keyed variant for functions you declare yourself:
//   cadence::hook_typed("MenuLayer::init()", my_init, &orig_init);
template <typename Detour, typename Original>
bool hook_typed(const char* sig, Detour detour, Original* original) {
    void* target = addr(sig);
    if (!target) return false;
    return hook(target, (void*)detour, (void**)original);
}

} // namespace cadence

// Exported from every mod that includes this header: the layout hashes of the
// GD classes whose headers the mod included. Empty if it included none.
extern "C" __declspec(dllexport) inline const char* mod_layout_hashes() {
    static const std::string json = [] {
        std::string s = "{";
        for (auto* r = cadence::detail::LayoutRegistration::head(); r; r = r->next) {
            if (s.size() > 1) s += ",";
            s += "\"" + std::string(r->cls) + "\":\"" + r->hash + "\"";
        }
        return s + "}";
    }();
    return json.c_str();
}

#endif // __cplusplus

// ── Mod lifecycle ─────────────────────────────────────────────────────

// Required: called once after the mod is loaded.
//   extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api);

// Optional: called before a hot reload (F6). Undo anything the loader cannot:
// nodes you added, flags you flipped, timers. Hooks are removed for you.
//   extern "C" __declspec(dllexport) void mod_unload();

// Generated by the build, not written by hand: mod_info() returns the name,
// author, description, key and version given to cadence_add_mod(), as JSON.
