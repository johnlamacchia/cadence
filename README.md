# Cadence

Minimal mod loader + SDK for Geometry Dash 2.2091 (Windows x64).

No utility libraries, no framework opinions. The loader does three things: loads mod DLLs, resolves GD function addresses, and installs hooks. Everything else is up to you.

## Quick start

### Requirements

- **Visual Studio 2022 Build Tools** with C++ desktop workload (MSVC v143, Windows SDK)
- **CMake 3.20+**
- **Python 3.10+** (for codegen only)

### Build

```powershell
# Generate bindings (one-time, or after updating .bro files)
cd codegen
python generate.py ..\..\bindings\bindings 2.2091

# Build everything — loader + all 16 mods
cd ..
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Output:
- `build/loader/Release/winmm.dll` — the loader
- `build/mods/<name>/Release/<name>.dll` — each mod

### Install

Copy to your GD folder (`C:\Program Files (x86)\Steam\steamapps\common\Geometry Dash`):

```
winmm.dll                           ← loader (from build/loader/Release/)
generated/addresses_2.2091.json     ← address database
generated/layout_hashes_2.2091.json ← class layout hashes
mods/                               ← create this folder
  example.dll                       ← any mod DLLs you want
  noclip.dll
  speedhack.dll
  ...
```

Launch GD normally. The loader logs everything to `loader.log` in the GD folder.

### Hot reload

Press **F6** in-game to reload all mods without restarting GD. The loader:
1. Calls `mod_unload()` on each mod (if exported)
2. Removes all hooks the mod installed
3. Frees the DLL
4. Reloads all DLLs from `mods/` and calls `mod_main()` again

This lets you recompile a mod, copy the DLL, and press F6 to test — no GD restart needed.

### Crash handler

If GD crashes, Cadence writes a crash report and minidump to the GD folder:
- `crash_YYYYMMDD_HHMMSS.log` — human-readable: exception type, registers, stack trace with module names and offsets
- `crash_YYYYMMDD_HHMMSS.dmp` — open in Visual Studio or WinDbg for full post-mortem debugging

### Uninstall

Delete `winmm.dll` from the GD folder. That's it. No registry changes, no system modifications.

---

## Write a mod

A mod is a standard Windows DLL that exports `mod_main`. That's the only requirement.

### Minimal example

```cpp
#include <sdk.h>

// Store the original function pointer so we can call through
static bool(__fastcall* orig_MenuLayer_init)(void* self);

// Your hook — same signature as the original, with __fastcall
static bool __fastcall hook_MenuLayer_init(void* self) {
    bool ret = orig_MenuLayer_init(self);  // call original first
    // ... do your thing after init ...
    return ret;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    // Resolve + hook in one call
    cadence::hook_typed("MenuLayer::init()",
        hook_MenuLayer_init, &orig_MenuLayer_init);
}
```

### CMake setup for your mod

Add a directory under `mods/` with a `CMakeLists.txt`:

```cmake
add_library(mymod MODULE main.cpp)
target_include_directories(mymod PRIVATE ${CMAKE_SOURCE_DIR}/loader/include)
set_target_properties(mymod PROPERTIES
    PREFIX ""
    OUTPUT_NAME "mymod"
    SUFFIX ".dll"
)
```

Then add `add_subdirectory(mods/mymod)` to the root `CMakeLists.txt`.

---

## SDK API reference

The entire public API lives in one header: `loader/include/sdk.h`.

### C++ API (recommended)

```cpp
#include <sdk.h>

// Must be called first in mod_main
cadence::set_api(api);

// Resolve a GD function address by its Broma signature string
void* addr = cadence::addr("PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)");

// Hook any function — returns true on success
// target:   address to hook
// detour:   your replacement function (must match calling convention)
// original: receives a trampoline pointer to call the real function
cadence::hook(target, (void*)my_detour, (void**)&orig_fn);

// Type-safe hook — resolves address + hooks in one call
// The original pointer type is inferred from the template
cadence::hook_typed("MenuLayer::init()", hook_init, &orig_init);

// Remove a hook (usually not needed — the loader removes hooks on unload)
cadence::unhook(target);

// Query GD version string
const char* ver = cadence::gd_version();  // e.g. "2.2091"
```

### C API

For C mods or non-C++ languages that can call C functions:

```c
void*       api->addr(api, "ClassName::funcName(params)");
bool        api->hook_fn(api, target, detour, &original);
bool        api->unhook_fn(api, target);
const char* api->gd_version_fn(api);
```

Convenience macros: `GD_ADDR(api, sig)`, `GD_HOOK(api, target, det, orig)`, `GD_UNHOOK(api, target)`, `GD_VERSION(api)`.

### Function signatures

Signatures match the keys in `generated/addresses_2.2091.json` exactly. The file contains **6855 resolved addresses** for GD 2.2091. Examples:

```
MenuLayer::init()
PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)
PlayLayer::onQuit()
PlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)
PlayLayer::resetLevel()
GJBaseGameLayer::update(float dt)
CCScheduler::update(float dt)              ← cocos2d (use GetProcAddress instead)
UILayer::init(GJBaseGameLayer* layer)
FLAlertLayer::create(...)
ButtonSprite::create(...)
```

For cocos2d functions (CCNode, CCDirector, CCScheduler, etc.), use `GetProcAddress` with the mangled export name from `libcocos2d.dll` — these are native DLL exports, not in the address JSON.

### Calling conventions

GD is compiled with MSVC x64. All functions use `__fastcall`:

| Type | `this` | Parameters |
|------|--------|------------|
| Member function | `rcx` | `rdx`, `r8`, `r9`, stack |
| Static function | — | `rcx`, `rdx`, `r8`, `r9`, stack |
| Virtual function | `rcx` | `rdx`, `r8`, `r9`, stack |

Your hook signature must match: `ReturnType __fastcall hook(void* self, ...)`.

**std::string by value**: MSVC passes `std::string` by value as a hidden pointer. If a GD function takes `std::string desc`, your hook takes `std::string* desc` (pointer). See the example mod's `FLAlertLayer::create` call.

### Hook chaining

Multiple mods can hook the same function. The hook engine chains them automatically — each mod's `original` pointer calls through to the next mod's detour, forming a chain that ends at the real function. No special code needed in your mod.

### Mod lifecycle

```cpp
// Required: called after the loader resolves addresses
extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api);

// Optional: called before your DLL is unloaded (hot reload or shutdown)
// Clean up threads, UI elements, timers. You do NOT need to unhook.
extern "C" __declspec(dllexport) void mod_unload();

// Optional: layout hash checking (see below)
extern "C" __declspec(dllexport) const char* mod_layout_hashes();
```

### Thread safety

If your mod creates background threads (e.g., for key polling), you **must**:

1. Use a `volatile bool g_running` flag
2. In `mod_unload()`, set `g_running = false` and `WaitForSingleObject(thread, 2000)` before returning
3. **Never** call GD/cocos2d functions from a background thread — cocos2d is single-threaded. Use a flag + main-thread dispatch pattern instead:

```cpp
static volatile bool g_toggle_pending = false;

// Background thread: only sets flags
static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('N') & 1) g_toggle_pending = true;
        Sleep(50);
    }
    return 0;
}

// Main thread hook: processes flags safely
static void __fastcall hook_drawScene(void* director) {
    if (g_toggle_pending) {
        g_toggle_pending = false;
        // Safe to call GD functions here — we're on the GL thread
        do_the_thing();
    }
    orig_drawScene(director);
}
```

### Hooking cocos2d functions

For cocos2d exports from `libcocos2d.dll`, use `GetProcAddress` with the MSVC-mangled name:

```cpp
HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");

// CCScheduler::update — runs every frame, good for per-frame logic
void* schedUpdate = (void*)GetProcAddress(cocos,
    "?update@CCScheduler@cocos2d@@UEAAXM@Z");

// CCDirector::drawScene — runs every frame, good for occasional dispatch
void* drawScene = (void*)GetProcAddress(cocos,
    "?drawScene@CCDirector@cocos2d@@QEAAXXZ");

// CCNode operations
void* addChild = GetProcAddress(cocos, "?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
void* setPosition = GetProcAddress(cocos, "?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
void* setVisible = GetProcAddress(cocos, "?setVisible@CCNode@cocos2d@@UEAAX_N@Z");
void* setScale = GetProcAddress(cocos, "?setScale@CCNode@cocos2d@@UEAAXM@Z");
void* removeChild = GetProcAddress(cocos, "?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");

// CCLabelBMFont::create
void* labelCreate = GetProcAddress(cocos,
    "?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");

// CCDirector::sharedDirector + getRunningScene (for adding UI to current scene)
void* sharedDirector = GetProcAddress(cocos,
    "?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
void* getRunningScene = GetProcAddress(cocos,
    "?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");

// CCMenu::create
void* menuCreate = GetProcAddress(cocos, "?create@CCMenu@cocos2d@@SAPEAV12@XZ");
```

### Layout hash checking

If your mod reads GD class member fields (offsets), export a layout hash so the loader can refuse to load your mod if class layouts change:

```cpp
extern "C" __declspec(dllexport) const char* mod_layout_hashes() {
    return R"({"PlayLayer":"a1b2c3d4","UILayer":"e5f6a7b8"})";
}
```

Get hash values from `generated/layout_hashes_2.2091.json`.

---

## Commonly used GD offsets

These are member offsets confirmed for GD 2.2091 via IDA analysis. Use layout hashes if you rely on them.

| Class | Offset | Type | Field |
|-------|--------|------|-------|
| PlayLayer | 0x3358 | bool | m_isPracticeMode |
| PlayLayer | 0x3870 | CCDrawNode* | debug draw node |
| UILayer | 0x280 | CCMenuItemSpriteExtra* | m_pauseBtn |

---

## Update after a GD patch

When GD updates (e.g. 2.2091 to 2.2092):

```powershell
python tools/sdk_update.py --old 2.2091 --new 2.2092
```

This:
1. Copies the new exe to `binaries/2.2092/`
2. Fingerprints and matches functions (~96% automated)
3. Regenerates headers if bindings exist
4. Shows which class layouts changed (update your offset assumptions!)
5. Lists unmatched functions for manual IDA work

### Add a missing address

If a function wasn't matched automatically, find it in IDA and add to `generated/addresses.local.json`:

```json
{
    "SomeClass::someFunc(int param)": "0x12ab34"
}
```

Local overrides merge at load time. Mods reference functions by signature string — never by baked-in address — so they keep working across GD versions as long as the address JSON is updated.

---

## How the proxy works

The loader ships as `winmm.dll` (a Windows multimedia DLL). When placed in the GD folder, Windows loads it instead of the system copy via DLL search order. All 180 winmm exports are forwarded to the real `C:\Windows\System32\winmm.dll` using MASM jump stubs. The loader initializes during `DllMain`, then GD starts normally with zero overhead.

The loader hooks nothing in GD itself (except `CCDirector::drawScene` for hot reload dispatch). Mods decide what to hook.

### Hook engine

The hook engine uses x64 trampoline hooking with [Zydis](https://github.com/zyantific/zydis) v4.1.0 for instruction decoding and relocation. Each hook:

1. Decodes enough instructions at the target to fit a 14-byte absolute jump (`ff 25` + 8-byte address)
2. Allocates a trampoline within +/-2GB of the target (for RIP-relative instruction relocation)
3. Copies and relocates the original instructions to the trampoline
4. Appends a jump back to the remaining original code
5. Overwrites the target with a jump to your detour

RIP-relative instructions (memory operands, branches, calls) are properly relocated. Conditional jumps use an inverted-condition-over-absolute-jump pattern.

---

## Project structure

```
cadence/
+-- binaries/                  versioned GD exe copies
+-- codegen/                   Broma parser + header/address generator
+-- generated/
|   +-- addresses_2.2091.json  6855 function addresses
|   +-- layout_hashes_*.json   class layout fingerprints
|   +-- include/gd/            generated C++ headers per GD class
+-- loader/
|   +-- include/sdk.h          entire public API (one file)
|   +-- src/
|       +-- loader.cpp         mod loading, address resolution, hot reload
|       +-- hook.cpp           x64 trampoline hook engine
|       +-- hook.h
|       +-- crash.cpp          vectored exception handler + minidump
|       +-- proxy.asm          180 winmm export forwarding stubs (MASM)
+-- mods/                      16 example mods (see below)
+-- tools/                     sdk update command
```

---

## Example mods

Cadence ships with 16 example mods covering common modding patterns. Drop any combination into your GD `mods/` folder:

| Mod | Description | Toggle | Default |
|-----|------------|--------|---------|
| `example` | Adds a "Hello" button to the main menu with an alert dialog | always on | ON |
| `fpsdisplay` | On-screen FPS counter (top-left, persists across scenes) | always on | ON |
| `noclip` | Prevents player death — pass through all obstacles | **N** | OFF |
| `speedhack` | Change game speed (0.25x to 5x) — shows in title bar | **+/-** speed, **0** reset | 1x |
| `practicemusic` | Plays normal music in practice mode instead of practice track | always on | ON |
| `instantrestart` | Press R to instantly restart the current level | **R** | ON |
| `hitboxes` | Toggle debug hitbox rendering for all game objects | **H** | OFF |
| `autoretry` | Auto-restart on death (skips the death animation) | **T** | OFF |
| `startposswitcher` | Quick level reset from current position | **[ / ]** | ON |
| `pausecountdown` | 3-2-1 countdown in the title bar after unpausing | always on | ON |
| `attemptcounter` | Tracks attempts per session, shows count on level quit | always on | ON |
| `anticheat` | Bypasses GD's spawn abuse check | always on | ON |
| `hidepause` | Hides the pause button for a cleaner HUD | **P** | OFF |
| `noglow` | Removes glitter/glow effects from objects | always on | ON |
| `hideground` | Hides ground and mid-ground decoration lines | **V** | OFF |
| `hideattempts` | Hides the attempt counter label in-game | always on | ON |

All mods with a toggle key can be toggled live during gameplay. Multiple mods can hook the same function — the hook engine chains them automatically.

### Patterns demonstrated

| Pattern | Mods using it | What it shows |
|---------|--------------|---------------|
| Hook a GD member function | all | Basic hooking with `cadence::hook_typed` |
| Hook a cocos2d export | fpsdisplay, speedhack, hitboxes | `GetProcAddress` + `cadence::hook` |
| Per-frame logic via CCScheduler::update | fpsdisplay, hitboxes, speedhack | Hooking the game's frame tick |
| Main-thread dispatch via CCDirector::drawScene | hideground, hidepause | Safe flag-based dispatch from bg thread |
| Background key polling thread | noclip, speedhack, hitboxes, hideground, hidepause | `GetAsyncKeyState` + thread safety |
| Creating UI (labels, buttons, menus) | example, fpsdisplay | cocos2d node creation and scene management |
| Scene change detection | fpsdisplay | Tracking `getRunningScene` to handle transitions |
| GD class member field access | hidepause, practicemusic | Reading/writing struct offsets |
| Intercepting + modifying args | hideground, speedhack | Changing function arguments before forwarding |
| Clean hot-reload support | all threaded mods | `g_running` flag + `WaitForSingleObject` in `mod_unload` |

---

## License

The bindings data (class names, function signatures, addresses) is derived from [geode-sdk/bindings](https://github.com/geode-sdk/bindings) under the Boost Software License 1.0. See `THIRD_PARTY_LICENSES.txt`.
