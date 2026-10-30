// gd-sdk public API
// This is the entire mod-facing interface. Read time: ~3 minutes.
//
// A mod is a DLL that exports:
//   extern "C" void mod_main(const LoaderAPI* api);
//
// The LoaderAPI lets you:
//   1. Resolve GD function addresses by signature string
//   2. Hook any resolved function (x64, chains correctly)
//   3. Unhook
//   4. Query the current GD version
//
// Functions are never resolved by baked-in address. If GD updates and
// addresses change, users swap the address JSON file and existing mod
// DLLs keep working without recompilation.

#pragma once
#include <cstdint>

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
};

// Convenience macros for the C API
#define GD_ADDR(api, sig)                 ((api)->addr((api), (sig)))
#define GD_HOOK(api, target, detour, orig) ((api)->hook_fn((api), (target), (detour), (orig)))
#define GD_UNHOOK(api, target)            ((api)->unhook_fn((api), (target)))
#define GD_VERSION(api)                   ((api)->gd_version_fn((api)))

// ── C++ type-safe layer (header-only) ─────────────────────────────────

#ifdef __cplusplus

namespace gdsdk {

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

// Type-safe hook helper.
// Usage:
//   static bool(__thiscall* orig_init)(MenuLayer*);
//   bool __fastcall hook_init(MenuLayer* self) {
//       if (!orig_init(self)) return false;
//       // add your stuff
//       return true;
//   }
//   gdsdk::hook_typed("MenuLayer::init()", hook_init, &orig_init);
template<typename Detour, typename Original>
bool hook_typed(const char* sig, Detour detour, Original* original) {
    void* target = addr(sig);
    if (!target) return false;
    return hook(target, (void*)detour, (void**)original);
}

} // namespace gdsdk

#endif // __cplusplus

// ── Mod metadata (optional, for layout hash checking) ─────────────────

// If your mod uses GD class layouts, export this so the loader can verify
// compatibility before calling mod_main:
//
//   extern "C" const char* mod_layout_hashes() {
//       return R"({"MenuLayer":"abcd1234","FLAlertLayer":"ef567890"})";
//   }
