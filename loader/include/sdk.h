// Cadence public API
// This is the entire mod-facing interface. Read time: ~3 minutes.
//
// A mod is a DLL that exports:
//   extern "C" void mod_main(const LoaderAPI* api);
//
// The LoaderAPI lets you:
//   1. Resolve GD function addresses by signature string
//   2. Resolve cocos2d exports by mangled name
//   3. Hook any resolved function (x64, chains correctly)
//   4. Unhook
//   5. Log to the shared loader.log
//   6. Query the current GD version

#pragma once
#include <cstdint>
#include <cstdarg>
#include <cstdio>

// ── C API ─────────────────────────────────────────────────────────────

struct LoaderAPI {
    // Resolve a GD function by its signature string.
    // Returns nullptr if the function has no known address.
    // Example: api->addr(api, "MenuLayer::init()")
    void* (*addr)(const LoaderAPI* self, const char* signature);

    // Install an x64 hook. *original receives a trampoline to the real function.
    // Multiple mods hooking the same function chain correctly.
    bool (*hook_fn)(const LoaderAPI* self, void* target, void* detour, void** original);

    // Remove a previously installed hook.
    bool (*unhook_fn)(const LoaderAPI* self, void* target);

    // Current GD version string (e.g. "2.2091").
    const char* (*gd_version_fn)(const LoaderAPI* self);

    // Resolve a cocos2d export by its MSVC-mangled name.
    // Equivalent to GetProcAddress(libcocos2d, name) but you don't
    // need to call GetModuleHandle yourself.
    // Example: api->cocos_addr(api, "?setVisible@CCNode@cocos2d@@UEAAX_N@Z")
    void* (*cocos_addr)(const LoaderAPI* self, const char* mangled_name);

    // Write a line to loader.log (printf-style formatting).
    void (*log_fn)(const LoaderAPI* self, const char* fmt, ...);
};

// Convenience macros for the C API
#define GD_ADDR(api, sig)                 ((api)->addr((api), (sig)))
#define GD_HOOK(api, target, detour, orig) ((api)->hook_fn((api), (target), (detour), (orig)))
#define GD_UNHOOK(api, target)            ((api)->unhook_fn((api), (target)))
#define GD_VERSION(api)                   ((api)->gd_version_fn((api)))
#define GD_COCOS(api, name)              ((api)->cocos_addr((api), (name)))

// ── C++ type-safe layer (header-only) ─────────────────────────────────

#ifdef __cplusplus

namespace cadence {

inline const LoaderAPI* g_api = nullptr;

inline void set_api(const LoaderAPI* api) { g_api = api; }

inline void* addr(const char* sig) {
    return g_api ? g_api->addr(g_api, sig) : nullptr;
}

inline bool hook(void* target, void* detour, void** original) {
    return g_api ? g_api->hook_fn(g_api, target, detour, original) : false;
}

inline bool unhook(void* target) {
    return g_api ? g_api->unhook_fn(g_api, target) : false;
}

inline const char* gd_version() {
    return g_api ? g_api->gd_version_fn(g_api) : "unknown";
}

inline void* cocos(const char* mangled_name) {
    return g_api ? g_api->cocos_addr(g_api, mangled_name) : nullptr;
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

// Type-safe hook helper.
// Usage:
//   static bool(__thiscall* orig_init)(MenuLayer*);
//   bool __fastcall hook_init(MenuLayer* self) {
//       if (!orig_init(self)) return false;
//       // add your stuff
//       return true;
//   }
//   cadence::hook_typed("MenuLayer::init()", hook_init, &orig_init);
template<typename Detour, typename Original>
bool hook_typed(const char* sig, Detour detour, Original* original) {
    void* target = addr(sig);
    if (!target) return false;
    return hook(target, (void*)detour, (void**)original);
}

} // namespace cadence

#endif // __cplusplus

// ── Mod lifecycle ─────────────────────────────────────────────────────

// Required: entry point called after the loader resolves addresses.
//   extern "C" void mod_main(const LoaderAPI* api);

// Optional: called before your DLL is unloaded during hot reload (F6).
// Clean up any state that would dangle (UI elements, timers, etc.).
// You do NOT need to unhook — the loader removes your hooks automatically.
//   extern "C" void mod_unload();

// Optional: export layout hashes so the loader can refuse your mod
// if class layouts changed between GD versions.
//   extern "C" const char* mod_layout_hashes() {
//       return R"({"MenuLayer":"abcd1234","FLAlertLayer":"ef567890"})";
//   }
