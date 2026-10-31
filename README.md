# Cadence

A minimal mod loader and SDK for **Geometry Dash 2.2091** (Windows x64).

The loader does three things: it loads mod DLLs, resolves GD function addresses by signature,
and installs hooks. No event system, no settings UI, no utility library, no wrapping of GD or
cocos2d. A mod is one DLL with one required export; the whole mod-facing API is one header
you can read in five minutes.

```cpp
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static void __fastcall my_reset(PlayLayer* self) {
    cadence::log("level reset, practice mode: %d", self->m_isPracticeMode);
    cadence::original<&PlayLayer::resetLevel>(self);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::resetLevel>(my_reset);   // wrong signature = compile error
}
```

- **[docs/GUIDE.md](docs/GUIDE.md)**: how to write a mod that does not crash GD (read this first)
- **[docs/ADDRESS_VERIFICATION.md](docs/ADDRESS_VERIFICATION.md)**: how many addresses check out against the real exe
- [Install](#install) · [Write a mod](#write-a-mod) · [API](#api) · [Updating GD](#updating-to-a-new-gd-version) · [Example mods](#example-mods) · [Tests](#tests-and-verification)

## What the loader does, and refuses to do

| Situation | What happens |
|---|---|
| Startup | Loads every `mods/*.dll`, alphabetically, calls `mod_main`. No manifest, no dependency resolution. |
| A function has no known address (missing, or the bindings mark it `win inline`) | That hook is skipped and a line goes to `loader.log`. GD is not touched. |
| An address is not inside `GeometryDash.exe`'s code (the bindings' `0x9999999` placeholder, a typo in a local override) | Ignored at load, with an example in `loader.log`. |
| A mod was built against a class whose layout changed | **The mod is refused, and the log names the class.** Other mods load. |
| Two mods hook the same function | They chain. Removing either one, in any order, leaves the other working. |
| A mod hook crashes GD | `crash_<time>.log` and a `.dmp` are written next to the exe. |

The loader hooks nothing in GD. Its one hook, on `CCDirector::drawScene`, exists only to run
hot reload on the game thread.

## Install

You need Geometry Dash 2.2091 on Windows.

**From a release zip**

```powershell
powershell -File install.ps1                          # auto-detects the Steam install
powershell -File install.ps1 -GdDir "D:\Games\Geometry Dash"
```

**From source**

Requirements: Visual Studio 2022 Build Tools (C++ desktop workload), CMake 3.20+, Python 3.10+.

```powershell
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
powershell -File tools/install.ps1
```

This puts `winmm.dll` (the loader), `generated/` (address and layout data) and the example mods
into your GD folder:

```
Geometry Dash/
  winmm.dll                          the loader
  generated/addresses_2.2091.json    function signature -> address
  generated/layout_hashes_2.2091.json
  generated/addresses.local.json     your own additions (optional)
  mods/*.dll                         your mods
  loader.log                         what the loader did (created at runtime)
```

Launch GD normally. **Uninstall:** `install.ps1 -Uninstall` (deletes `winmm.dll`; no registry or
system changes).

## Write a mod

```powershell
python tools/new_mod.py my_mod                    # scaffolds mods/my_mod/
cmake -B build -A x64
cmake --build build --config Release --target my_mod_mod
copy build\mods\my_mod\Release\my_mod.dll "<Geometry Dash>\mods\"
```

Press **F6** in game to load it. Edit, rebuild, copy the DLL over the old one (it is not locked),
press F6 again. To develop outside this repo, copy `template/` and configure it with
`-DCADENCE_SDK_DIR=<path to cadence>`.

A mod's `CMakeLists.txt` is one call. Its arguments become the mod's metadata, baked into the
DLL by the build, so there is nothing to keep in sync by hand:

```cmake
cadence_add_mod(my_mod main.cpp NAME "My Mod" AUTHOR "me" DESCRIPTION "Does a thing" KEY "M" VERSION "1.0")
```

Every directory under `mods/` with a `CMakeLists.txt` is built automatically.

## API

All of it is in [`loader/include/sdk.h`](loader/include/sdk.h).

### What the loader gives you (`LoaderAPI`)

| C API | `namespace cadence` | |
|---|---|---|
| `addr(api, "Class::fn(args)")` | `addr("...")` | Address of a GD function; the string is a key in `addresses_<ver>.json`. `nullptr` and a log warning if unknown. |
| `hook_fn(api, target, detour, &orig)` | `hook(target, detour, &orig)` | Install an x64 hook. `orig` receives the next function in the chain. |
| `unhook_fn(api, target)` | `unhook(target)` | Remove *your* hooks on `target`. You rarely need it: reload removes them. |
| `gd_version_fn(api)` | `gd_version()` | `"2.2091"`. |
| `log_fn(api, fmt, ...)` | `log(fmt, ...)` | A printf-style line in `loader.log`. |
| `get_mods(api, &n)` | `get_mods(&n)` | The loaded mods and their metadata. |

### Typed layer (header-only, compiles down to the calls above)

```cpp
cadence::hook<&PlayLayer::resetLevel>(detour);     // detour: void(__fastcall*)(PlayLayer*), checked at compile time
cadence::original<&PlayLayer::resetLevel>(self);   // the next function in the chain, typed
auto f = cadence::fn<&PlayLayer::resetLevel>();    // the game's own function, to call it
```

`cadence::hook_typed("Class::fn(args)", detour, &orig)` is the string-keyed variant for functions
you declare yourself. `cadence::cocos("?mangled@name@@...")` is `GetProcAddress` on `libcocos2d.dll`.

### Optional helpers (`sdk_extras.h`; the loader and `sdk.h` do not need them)

`cadence::play_layer()` returns the live `PlayLayer*` (nullptr outside a level, never stale);
`cadence::key_pressed(vk)` is a focus-aware, once-per-press key check that needs no thread.

### Your DLL's exports

| Export | Required | Purpose |
|---|---|---|
| `void mod_main(const LoaderAPI*)` | yes | Called once after load. Call `cadence::set_api(api)` first. |
| `void mod_unload()` | no | Called before a reload. Undo anything the loader cannot (hidden nodes, flipped flags). Hooks are removed for you. |
| `mod_info()`, `mod_layout_hashes()` | generated | Metadata and the layout hashes of the classes you included. Never written by hand. |

## Bindings and generated files

`codegen/` reads the Broma bindings (`geode-sdk/bindings`, BSL-1.0; no Geode code is used) and writes,
under `generated/`:

| File | Contents |
|---|---|
| `addresses_<ver>.json` | Signature -> Windows address (6,841 for 2.2091). Functions marked `win inline` are not hookable and are left out, as are addresses that cannot be function starts: the bindings' `0x9999999` placeholder (35) and addresses that fall in the middle of another function according to the exe's own unwind table (3, all `CCTexturePVR`). |
| `addresses_rejected_<ver>.json` | The addresses dropped by that check, and why. |
| `layout_hashes_<ver>.json` | A hash of each class's exact byte layout (size, every member's offset and type). |
| `layout_report_<ver>.json` | Per class: `verified` / `tail-padded` / `unverified` / `incomplete`, and why. |
| `class_sizes_<ver>.json` | `sizeof` of 185 classes, read out of the game binary. |
| `include/gd/**` | One header per class: declarations and member layout only, with `static_assert`s on `sizeof` and every member's offset. |

The headers are compiled as part of the tests, so a mistake in the layout model fails the build
instead of showing up in game. How far to trust a layout, by status, is in
[docs/GUIDE.md](docs/GUIDE.md#class-layouts-and-how-far-to-trust-them).

cocos2d: `libcocos2d.dll` exports its functions by name, so use `cadence::cocos()`. The bindings'
cocos2d classes are used for layout and inheritance only.

### Verification

`tools/layout_check.py` and `tools/verify_addresses.py` check the generated data against the real
`GeometryDash.exe`; results are in [docs/ADDRESS_VERIFICATION.md](docs/ADDRESS_VERIFICATION.md). In
short, for 2.2091: every address is inside the exe's code, a random sample of 400 is 100% referenced
and 100% decodes as code, and the code behaves like its name in 99.3% of `create` functions and
99.9% of vtable neighbourhoods. The same check found 38 addresses in the bindings that were wrong
(placeholders and mid-function), now dropped.

## Updating to a new GD version

```powershell
python tools/sdk_update.py --old 2.2091 --new 2.2092
```

One command that:

1. copies the new `GeometryDash.exe` to `binaries/2.2092/`;
2. runs [gdport](gdport/README.md) to carry every address over (fingerprinting each function and
   matching by strings, exact bytes, instruction shape and call graph, then validating);
3. reads each class's `sizeof` out of the new exe;
4. regenerates the headers and layout hashes (from the new bindings if they exist, otherwise the old layouts are
   carried over and flagged);
5. prints which class layouts changed and which functions it could not place, for a human to find in IDA.

gdport places a function only when one candidate fits and validates every match. Measured on the
6,841 functions here (`python gdport/selftest.py`): **0 wrong placements** whether the new exe is
identical or has an instruction changed in 30% of its functions, 99.2% of functions placed, the
rest listed for a human. Details and limits in [gdport/README.md](gdport/README.md); in short, it has
been tested on identical and altered copies of 2.2091, not yet on two real GD releases (none has
come out since 2.2091 was installed here), so run `tools/verify_addresses.py` on a real result.

### Add a missing address yourself

If a function was not placed (or the bindings do not have it), find it in IDA and add it to
`generated/addresses.local.json`:

```json
{ "SomeClass::someFunc(int param)": "0x12ab34" }
```

The loader merges this file over the official one at startup, so nobody waits for an update. The
address must point into `GeometryDash.exe`'s code or it is ignored with a warning.

## Example mods

Press keys while **in a level** with GD focused unless noted.

| Mod | What it does | Key |
|---|---|---|
| `example` | Clicking the RobTop logo on the main menu shows "Hello" in GD's own `FLAlertLayer` | click |
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

The example mod hooks `MenuLayer::onRobTop` rather than `onMoreGames`: the bindings give
`onMoreGames` no Windows address in 2.2091, so it cannot be hooked (trying to is a compile error that says so).

## Tests and verification

```powershell
cmake --build build --config Release
ctest --test-dir build -C Release                                  # hook engine + loader smoke test
cmake --build build --config Release --target gd_headers_check     # compiles every generated header and its static_asserts
python tools/layout_check.py check <bindings>                       # computed layouts vs the game binary
python tools/verify_addresses.py --sample 400                       # addresses vs the game binary
```

- `hook_test`: chaining; removing a hook from the head, middle or tail of a chain; byte-exact restore; re-hooking.
- `loader_smoke`: loads the real loader and all 17 mods in a scratch folder: load order, shadow copies, missing and
  invalid addresses skipped without touching memory, a stale layout hash refusing exactly the mods that use the class.
- `gd_headers`: all 1,104 generated headers compile under MSVC, with every `static_assert` passing.
- `python gdport/selftest.py <exe> <addresses.json>`: the address porter against known truth (identical and altered exe).

**Not covered by automated tests:** anything that needs the game running. The mods themselves, hot reload (F6),
and the hook engine against real GD functions are checked by hand in game.

## How it works

**The proxy.** The loader is `winmm.dll`. GD always loads `fmod.dll`, which imports `winmm.dll`; `winmm.dll` is not in
Windows' KnownDLLs list, so a copy next to `GeometryDash.exe` is found before the one in System32. All 180 winmm
exports are forwarded to the real DLL through MASM jump stubs, and the loader initialises in `DllMain`. The only other
proxyable system DLL GD imports is `xinput1_4.dll` (via `libcocos2d.dll`); it would also work and has far fewer exports.

**The hook engine** (`loader/src/hook.cpp`) is an x64 trampoline hook, using [Zydis](https://github.com/zyantific/zydis)
to decode and relocate instructions:

1. Decode enough instructions at the target to fit a 14-byte absolute jump.
2. Allocate a trampoline within +/-2 GB, copy and relocate those instructions into it (RIP-relative operands and
   branches are rewritten), and jump back into the rest of the function.
3. Overwrite the target with a jump to the newest detour.

Hooks on one function form a chain per target: each hook's `original` points at the next older hook, the oldest at
the trampoline. Removing a hook from anywhere re-points the hooks above it, and removing the last one restores the
game's original bytes exactly. The trampoline is kept, so a function can be hooked again after a full unhook.

**Hot reload** (F6, for development): each mod's `mod_unload()` runs, its hooks are removed, and every `mods/*.dll` is
copied to `mods/.cadence-cache/` and loaded from the copy, so the originals are never locked. Old copies stay mapped
until exit, because their detours can still be on the call stack and UI they built can still point into them.

## Limits

- Geometry Dash **2.2091** only. Other versions need `sdk_update.py` and the matching bindings.
- Layouts: see the table in the guide. Most classes' layouts come from the bindings and are not independently verified.
- Virtual calls through the headers rely on the bindings listing virtual functions in vtable order; not verified class by class.
- Build mods in **Release**; the headers refuse the debug STL.

## Project layout

```
loader/            winmm proxy, mod loader, hook engine, crash handler
  include/sdk.h        the entire mod-facing API
  include/sdk_extras.h optional helpers
mods/              17 example mods (one directory each)
template/          standalone starter mod
cmake/             cadence_add_mod() build helper
codegen/           Broma parser, layout calculator, header + data generator
generated/         addresses, layout hashes and report, class sizes, include/gd/** headers
gdport/            address porting between GD versions
tests/             hook engine, loader smoke test, header compile check
tools/             new_mod, sdk_update, layout_check, verify_addresses, offset_probe, install, package
docs/              GUIDE.md, ADDRESS_VERIFICATION.md
```

`powershell -File tools/package.ps1` builds everything, runs the tests and writes `dist/Cadence-2.2091.zip`.

## License

The bindings data (class names, signatures, member lists, addresses, enums) is derived from
[geode-sdk/bindings](https://github.com/geode-sdk/bindings) under the Boost Software License 1.0.
See `THIRD_PARTY_LICENSES.txt`.
