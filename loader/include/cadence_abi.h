// The binary interface between the loader and mods. The loader and sdk.h both
// include this, so the two sides cannot drift apart. Mods should include sdk.h.
#pragma once

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
