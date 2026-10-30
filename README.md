# Cadence

Minimal mod loader for Geometry Dash 2.2091 (Windows x64).

No utility libraries, no framework opinions. The loader does three things: loads mod DLLs, resolves GD function addresses, and installs hooks. Everything else is up to you.

## Quick start

### Build

Requirements: Visual Studio 2022 Build Tools (C++ workload), CMake 3.20+, Python 3.10+.

```powershell
# Generate bindings (one-time, or after updating .bro files)
cd codegen
python generate.py ..\..\bindings\bindings 2.2091

# Build the loader and example mod
cd ..
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### Install

Copy these files to your GD folder (`C:\Program Files (x86)\Steam\steamapps\common\Geometry Dash`):

```
winmm.dll             ← the loader (from build/loader/Release/)
generated/            ← address files (addresses_2.2091.json, layout_hashes_2.2091.json)
mods/
  example.dll         ← example mod (from build/mods/example/Release/)
```

Launch GD. The loader logs to `loader.log` in the GD folder.

### Hot reload

Press **F6** in-game to reload all mods without restarting GD. The loader calls `mod_unload()` (if exported), removes all hooks the mod installed, unloads the DLL, then reloads and calls `mod_main()` again.

### Crash handler

If GD crashes, Cadence writes a crash report and minidump to the GD folder:
- `crash_YYYYMMDD_HHMMSS.log` — human-readable: exception type, registers, stack trace with module names
- `crash_YYYYMMDD_HHMMSS.dmp` — open in Visual Studio or WinDbg for full debugging

### Uninstall

Delete `winmm.dll` from the GD folder. That's it.

## Write a mod

A mod is a DLL that exports one function:

```cpp
#include <sdk.h>

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    // Resolve a GD function by its signature
    void* addr = cadence::addr("MenuLayer::init()");

    // Hook it
    static bool(__fastcall* original)(void* self);
    cadence::hook(addr, (void*)my_hook, (void**)&original);
}
```

### The API

```c
// Resolve a function address by signature string
void* api->addr(api, "ClassName::funcName(params)")

// Hook a function (x64 trampoline, chains correctly with other mods)
bool api->hook_fn(api, target, detour, &original)

// Unhook
bool api->unhook_fn(api, target)

// Get current GD version
const char* api->gd_version_fn(api)
```

Function signatures match the address JSON keys exactly. Open `generated/addresses_2.2091.json` to see all available signatures.

### Calling conventions

GD is compiled with MSVC. On x64 Windows:
- Non-static member functions: `__fastcall` with `this` in `rcx`
- Static/free functions: `__fastcall` (standard x64 calling convention)

Your hook must match: `ReturnType __fastcall hook(ClassName* self, ...)`.

### Layout hash checking

If your mod accesses GD class members (fields, not just functions), export a hash table so the loader can refuse your mod if class layouts changed:

```cpp
extern "C" __declspec(dllexport) const char* mod_layout_hashes() {
    return R"({"MenuLayer":"a1b2c3d4","FLAlertLayer":"e5f6a7b8"})";
}
```

Get hash values from `generated/layout_hashes_<version>.json`.

## Update after a GD patch

When GD updates (e.g. 2.2091 → 2.2092):

```powershell
# Port addresses to the new version
python tools/sdk_update.py --old 2.2091 --new 2.2092
```

This:
1. Copies the new exe to `binaries/2.2092/`
2. Fingerprints and matches functions (~96% automated)
3. Regenerates headers if bindings exist
4. Shows which class layouts changed
5. Lists unmatched functions for manual IDA work

### Add a missing address

If a function you need wasn't matched automatically, find it in IDA and add it to `generated/addresses.local.json`:

```json
{
    "SomeClass::someFunc(int param)": "0x12ab34"
}
```

Local overrides are merged at load time. Existing mods keep working without recompilation — they reference functions by signature, not by baked-in address.

## How the proxy works

The loader ships as `winmm.dll` (a Windows multimedia DLL). When placed in the GD folder, Windows loads it instead of the system copy. All 180 winmm exports are forwarded to the real `C:\Windows\System32\winmm.dll` at runtime. The loader runs its initialization in DllMain, then GD starts normally.

The loader hooks nothing in GD itself — it only loads mod DLLs from `mods/` and gives them the API. Mods decide what to hook.

## Project structure

```
cadence/
├── binaries/           versioned GD exe copies
├── codegen/            Broma parser + header/address generator
├── generated/          output: headers, address JSON, layout hashes
├── gdport/             function address porting tool
├── loader/             the proxy DLL (winmm.dll)
│   ├── include/sdk.h   entire public API
│   └── src/            proxy, hook engine, mod loader
├── mods/               example mods (see below)
└── tools/              sdk update command
```

## Example mods

Cadence ships with 16 example mods demonstrating the SDK. Drop any combination into `mods/`:

| Mod | What it does | Key |
|-----|-------------|-----|
| `example` | Adds a "Hello" button to the main menu | — |
| `fpsdisplay` | Live FPS counter in the window title bar | — |
| `noclip` | Prevents player death | N |
| `speedhack` | Change game speed | +/- |
| `practicemusic` | Normal music in practice mode | — |
| `instantrestart` | Press R to restart the current level | R |
| `hitboxes` | Toggle debug hitbox rendering | H |
| `autoretry` | Auto-restart on death (no death screen) | T |
| `startposswitcher` | Quick level reset | [ / ] |
| `pausecountdown` | 3-2-1 countdown after unpausing | — |
| `attemptcounter` | Tracks session attempts, shows on quit | — |
| `anticheat` | Bypasses the spawn abuse check | — |
| `hidepause` | Hides the pause button UI | P |
| `noglow` | Removes glitter/glow effects | — |
| `hideground` | Hides ground and mid-ground lines | V |
| `hideattempts` | Hides the attempt counter label | — |

Multiple mods can hook the same function — the hook engine chains them automatically.

## License

The bindings data (class names, function signatures, addresses) is derived from [geode-sdk/bindings](https://github.com/geode-sdk/bindings) under the Boost Software License 1.0. See `THIRD_PARTY_LICENSES.txt`.
