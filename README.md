# Cadence

A minimal mod loader and SDK for **Geometry Dash 2.2091** (Windows x64).

The loader does three things: it loads mod DLLs, resolves GD function addresses by
signature, and installs hooks. No event system, no settings UI, no utility library. A mod
is one DLL with one required export, and the whole mod-facing API is one header.

```cpp
#include <sdk.h>

static void(__fastcall* orig_resetLevel)(void* self);
static void __fastcall hook_resetLevel(void* self) {
    cadence::log("level reset");
    orig_resetLevel(self);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook_typed("PlayLayer::resetLevel()", hook_resetLevel, &orig_resetLevel);
}
```

- **[docs/GUIDE.md](docs/GUIDE.md)**: how to write a mod that does not crash GD (read this first)
- **[Write a mod in five minutes](#write-a-mod)** and the **[API reference](#api-reference)**
- **[17 example mods](#example-mods)** covering the common patterns

## Install

You need Geometry Dash 2.2091 on Windows.

**From a release zip**

```powershell
powershell -File install.ps1                          # auto-detects the Steam install
powershell -File install.ps1 -GdDir "D:\Games\Geometry Dash"
```

**From source**

Requirements: Visual Studio 2022 Build Tools (C++ desktop workload), CMake 3.20+.

```powershell
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
powershell -File tools/install.ps1
```

This puts `winmm.dll` (the loader), `generated/` (the address database) and the example
mods into your GD folder:

```
Geometry Dash/
  winmm.dll                        the loader
  generated/addresses_2.2091.json  6855 function addresses
  generated/layout_hashes_2.2091.json
  mods/*.dll                       your mods
  loader.log                       what the loader did (created at runtime)
```

Launch GD normally. **Uninstall:** `install.ps1 -Uninstall` (deletes `winmm.dll`; no registry
or system changes).

## Write a mod

```powershell
python tools/new_mod.py my_mod                    # scaffolds mods/my_mod/
cmake -B build -A x64
cmake --build build --config Release --target my_mod_mod
copy build\mods\my_mod\Release\my_mod.dll "<Geometry Dash>\mods\"
```

Press **F6** in game to load it. Edit, rebuild, copy the DLL over the old one (it is not
locked), press F6 again. To develop outside this repo, copy `template/` and configure it with
`-DCADENCE_SDK_DIR=<path to cadence>`.

Every directory under `mods/` with a `CMakeLists.txt` is built automatically; a mod's
CMakeLists is one line: `cadence_add_mod(my_mod main.cpp)`.

## API reference

All of it is in [`loader/include/sdk.h`](loader/include/sdk.h).

### Lifecycle exports (your DLL)

| Export | Required | Purpose |
|---|---|---|
| `void mod_main(const LoaderAPI*)` | yes | Called once after load. Call `cadence::set_api(api)` first. |
| `void mod_unload()` | no | Called before a reload. Undo anything the loader cannot (hidden nodes, flipped flags). Hooks are removed for you. |
| `const char* mod_info()` | no | JSON for the mod menu: `{"name","author","desc","key","ver"}`, all optional. |
| `const char* mod_layout_hashes()` | no | `{"PlayLayer":"<hash>"}`; the loader refuses the mod if a class layout changed. |

### Functions (`namespace cadence`)

| Call | What it does |
|---|---|
| `addr("Class::fn(args)")` | Address of a GD function; the string is a key in `addresses_2.2091.json`. `nullptr` (and a log warning) if unknown. |
| `hook(target, detour, &orig)` | Install a hook. `orig` receives the next function in the chain. Hooks from many mods on one function chain automatically. |
| `hook_typed("Class::fn(args)", detour, &orig)` | `addr` + `hook` in one call. |
| `unhook(target)` | Remove *your* hooks on `target`. Rarely needed: reload removes them. |
| `cocos("?mangled@name@@...")` | Export from `libcocos2d.dll` (CCNode, CCDirector, CCScheduler, ...). |
| `play_layer()` | The live `PlayLayer*`, or `nullptr` outside a level. Never stale. Game thread only. |
| `key_pressed(vk)` | True once per physical press, only while GD is focused. Call once per key per frame from a game-thread hook. |
| `log(fmt, ...)` | printf-style line in `loader.log`. |
| `gd_version()` | `"2.2091"`. |
| `get_mods(&count)` | The loaded mods: `filename, name, author, description, keybind, version, enabled`. |
| `off::...` | Verified field offsets (see the guide). |

The same functions are available as a C API on `LoaderAPI` (`addr`, `hook_fn`, `unhook_fn`,
`cocos_addr`, `log_fn`, `gd_version_fn`, `get_mods`) with `GD_ADDR` / `GD_HOOK` / ... macros.

### Calling convention

GD is MSVC x64. A hook is `Ret __fastcall hook(void* self, args...)`. `std::string` by value
is passed as `std::string*`. See the guide for the other ABI traps.

## Hot reload and crash reports

- **F6** unloads and reloads all mods without restarting. Mods are loaded from copies in
  `mods/.cadence-cache/`, so `mods/*.dll` is never locked and can be rebuilt while GD runs.
  Hook chains are relinked correctly when a mod in the middle is removed. Details in the
  [guide](docs/GUIDE.md#hot-reload).
- On a fatal exception the loader writes `crash_<time>.log` (exception, registers, stack with
  module+offset) and a `.dmp` into the GD folder.

## Example mods

Press keys while **in a level** with GD focused unless noted.

| Mod | What it does | Key |
|---|---|---|
| `example` | "Hello" button on the main menu that opens an alert | button |
| `modmenu` | In-game overlay listing every loaded mod with its keybind | **Insert** |
| `fpsdisplay` | FPS counter, top-left, on every scene | always on |
| `hitboxes` | Show the game's debug hitboxes | **H** |
| `noclip` | Survive all hazards | **N** |
| `autoretry` | Restart immediately on death | **T** |
| `speedhack` | Game speed 0.25x to 5x (title bar shows it) | **+** / **-** / **0** |
| `startposswitcher` | Restart the level | **[** or **]** |
| `instantrestart` | Restart the level | **R** |
| `hideground` | Hide ground and mid-ground lines | **V** |
| `hidepause` | Hide the on-screen pause button | **P** |
| `pausecountdown` | 3-2-1 on screen after pressing Resume | always on |
| `practicemusic` | Normal music in practice mode | always on |
| `attemptcounter` | Counts attempts, shows the total in the title bar on quit | always on |
| `noglow` | Remove glitter/glow effects | always on |
| `hideattempts` | Hide the in-level attempt label | always on |
| `anticheat` | Bypass the spawn-abuse check | always on |

| Pattern | Mods |
|---|---|
| Hook a GD member function | most of them |
| Hook a cocos2d export | `fpsdisplay`, `speedhack`, any `drawScene` user |
| Per-frame logic + keys on the game thread | `noclip`, `autoretry`, `hitboxes`, `speedhack` |
| Defer an action to the next frame | `autoretry` |
| Ask the game for the level, never cache it | `hitboxes`, `hideground`, `startposswitcher` |
| Timer without blocking | `pausecountdown` |
| Build UI (labels, panel, button) | `fpsdisplay`, `modmenu`, `pausecountdown`, `example` |
| Read/write a game field via a verified offset | `practicemusic`, `hidepause` |
| Undo your changes in `mod_unload` | `hitboxes`, `hideground`, `hidepause`, `speedhack` |

## Updating after a GD patch

```powershell
python tools/sdk_update.py --old 2.2091 --new 2.2092   # re-match addresses (~96% automatic)
python tools/offset_probe.py --verify                  # re-check every cadence::off offset
```

Functions that do not match automatically can be added by hand in
`generated/addresses.local.json` (merged at load):

```json
{ "SomeClass::someFunc(int param)": "0x12ab34" }
```

Mods refer to functions by signature, never by a baked-in address, so they keep working across
versions as long as the address file is updated.

To regenerate bindings and headers from `.bro` files: `python codegen/generate.py <bindings dir> 2.2091`.

## Tests

```powershell
cmake --build build --config Release
ctest --test-dir build -C Release
```

`hook_test` exercises the hook engine without launching GD: chaining, removing a hook from the
head / middle / tail of a chain, byte-exact restore, and re-hooking after a full unhook.

What is **not** covered by automated tests: anything that needs the game running (the mods
themselves, address resolution against a live process). Those are checked by hand in game; the
field offsets are checked against the binary with `tools/offset_probe.py --verify`.

## How it works

**The proxy.** The loader is `winmm.dll`. Windows loads it from the GD folder instead of the
system copy; all 180 winmm exports are forwarded to `System32\winmm.dll` through MASM jump
stubs. It initialises in `DllMain`, then GD starts normally.

**The hook engine** (`loader/src/hook.cpp`) is an x64 trampoline hook using
[Zydis](https://github.com/zyantific/zydis) to decode and relocate instructions:

1. Decode enough instructions at the target to fit a 14-byte absolute jump.
2. Allocate a trampoline within +/-2 GB, copy and relocate those instructions into it
   (RIP-relative operands and branches are rewritten), and jump back to the rest of the function.
3. Overwrite the target with a jump to your detour.

Hooks on the same function form a chain per target: each hook's `original` points at the next
older hook, and the oldest points at the trampoline. Removing a hook from anywhere in the chain
re-points the hooks above it, and removing the last one restores the game's original bytes
exactly. The trampoline is kept, so a function can be hooked again after a full unhook.

**Address resolution.** `generated/addresses_<ver>.json` maps Broma signatures to RVAs;
the loader rebases them at startup.

## Project layout

```
loader/            winmm proxy, mod loader, hook engine, crash handler
  include/sdk.h    the entire mod-facing API
mods/              17 example mods (one directory each)
template/          standalone starter mod
cmake/             cadence_add_mod() build helper
tests/             hook engine tests
tools/             new_mod.py, offset_probe.py, sdk_update.py, install.ps1, package.ps1
codegen/           Broma parser, header and address generator
generated/         address database, layout hashes, generated headers
docs/GUIDE.md      mod author's guide
```

`powershell -File tools/package.ps1` builds everything, runs the tests, and writes
`dist/Cadence-2.2091.zip` (loader, address files, all mods, SDK header, template, docs, installer).

## License

The bindings data (class names, function signatures, addresses) is derived from
[geode-sdk/bindings](https://github.com/geode-sdk/bindings) under the Boost Software License 1.0.
See `THIRD_PARTY_LICENSES.txt`.
