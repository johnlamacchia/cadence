# Cadence mod guide

Everything you need to write a mod that does not crash the game. The API itself is
small (`loader/include/sdk.h`, about 200 lines); most of this page is about how
Geometry Dash behaves, because that is where mods go wrong.

- [Your first mod](#your-first-mod)
- [The rules](#the-rules)
- [Recipes](#recipes)
- [Calling-convention and ABI pitfalls](#calling-convention-and-abi-pitfalls)
- [Reading and writing game fields](#reading-and-writing-game-fields)
- [Hot reload](#hot-reload)
- [Debugging](#debugging)
- [After a GD update](#after-a-gd-update)

---

## Your first mod

```powershell
python tools/new_mod.py my_mod                 # creates mods/my_mod/
cmake -B build -A x64
cmake --build build --config Release --target my_mod_mod
copy build\mods\my_mod\Release\my_mod.dll "<Geometry Dash>\mods\"
```

Press **F6** in game to load it. After that, edit, rebuild, copy the DLL over the old one
(it is not locked, see [Hot reload](#hot-reload)) and press F6 again.

Prefer to keep your mod outside this repo? Copy `template/` anywhere, rename `MY_MOD`,
and configure with `-DCADENCE_SDK_DIR=<path to cadence>` (or the `sdk/` folder of a release zip).

A complete mod is this:

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

Finding a function's signature: search `generated/addresses_2.2091.json`. The key is the
exact string you pass to `cadence::addr` / `hook_typed`.

---

## The rules

These are not style advice. Each one is a crash we hit and fixed.

### 1. Game and cocos2d calls only on the game thread

cocos2d is single-threaded. Calling `resetLevel`, `addChild`, `setVisible`, etc. from a
thread you created corrupts state or crashes at a random later point.

Do per-frame work in a hook that already runs on the game thread. `CCDirector::drawScene`
runs once per frame:

```cpp
static void(__fastcall* orig_drawScene)(void* director);
static void __fastcall hook_drawScene(void* director) {
    // safe to call game functions here
    orig_drawScene(director);
}
// mod_main:
cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
              (void*)hook_drawScene, (void**)&orig_drawScene);
```

You almost never need your own thread. If you do create one, you must stop it and
`WaitForSingleObject` it in `mod_unload`, and it must never touch the game.

### 2. Never cache the current PlayLayer: ask for it

A pointer you stored in `PlayLayer::init` and cleared in `PlayLayer::onQuit` can outlive the
level: two of our own mods did exactly that and crashed inside the game with a freed
`PlayLayer` (the crash reports showed a garbage vtable). Hooking `init`/`onQuit` to track
the level is unreliable, so ask the game instead:

```cpp
void* pl = cadence::play_layer();   // nullptr outside a level, never stale
if (!pl) return;
```

`play_layer()` reads `GameManager::m_playLayer`, which the game clears in `~PlayLayer`.
Use it every time you need the level; do not store the result across frames.

The same applies to any other node you hold (labels, buttons, scenes): a scene change frees
its children. Re-create UI after scene changes, and only call `removeChild` on a node whose
scene is still the running scene.

### 3. Do not re-enter the thing that is calling you

Calling `PlayLayer::resetLevel` from inside a `destroyPlayer` hook restarts the level while
the death is still being processed, which kills the player again, forever. Set a flag and act
on the next frame:

```cpp
static bool g_reset_pending = false;
static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    orig_destroyPlayer(self, player, object);
    g_reset_pending = true;
}
static void __fastcall hook_drawScene(void* director) {
    if (g_reset_pending) {
        g_reset_pending = false;
        if (void* pl = cadence::play_layer()) resetLevel(pl);
    }
    orig_drawScene(director);
}
```

### 4. Never block a hook

`Sleep()` in a hook that runs on the game thread freezes the whole game. For timers, record
`QueryPerformanceCounter` and compare it on later frames (see `mods/pausecountdown`).

### 5. Poll keys with `cadence::key_pressed`

```cpp
if (cadence::play_layer() && cadence::key_pressed('N')) { ... }
```

`key_pressed` fires once per physical press, ignores input while GD is not the foreground
window, and needs no thread. Call it once per key per frame from a game-thread hook.
Do not use `GetAsyncKeyState(key) & 1`: the low bit is not reliable and it fires when GD is
in the background.

Gate toggles on `cadence::play_layer()` if the key could also be typed into a text field
(level search, comments).

### 6. Always call the original (unless you mean not to)

Skipping `orig_*` replaces the function. That is the point of a noclip mod's
`destroyPlayer` hook, but it is a bug in almost every other hook. Hooks from several mods
chain automatically, and a mod that does not call through breaks every mod hooked after it.

### 7. Own your state in `mod_unload`

The loader removes your hooks on reload. It cannot know what else you changed. Anything you
altered in the game (hidden nodes, flags you flipped, the window title) belongs in
`mod_unload`, so a reload leaves the game as you found it.

---

## Recipes

Each of these exists as a working mod in `mods/`.

| I want to... | Do this | Example |
|---|---|---|
| Change what a function returns | Hook it, return your value, optionally call `orig` | `hitboxes` (`shouldDrawDebugHitboxes`) |
| Stop something happening | Hook it and `return` without calling `orig` | `noclip`, `anticheat` |
| Change an argument | Call `orig` with a different value | `hideground`, `noglow` |
| React to something happening | Call `orig`, then run your code | `attemptcounter` |
| Run every frame | Hook `CCDirector::drawScene` or `CCScheduler::update` | `autoretry`, `speedhack` |
| Respond to a key | `cadence::key_pressed` in a `drawScene` hook, or hook `UILayer::keyDown` | `noclip`, `instantrestart` |
| Show UI | `CCLabelBMFont::create`, `addChild` on the running scene, from the game thread | `fpsdisplay`, `pausecountdown`, `modmenu` |
| Add a button to a layer | Hook the layer's `init`, build with `CCMenuItemSpriteExtra::create` | `example` |
| Read what other mods are loaded | `cadence::get_mods` | `modmenu` |
| Hide an existing node | Reach it through a field offset, `setVisible(false)` | `hidepause` |

Hook a cocos2d function with `cadence::cocos("<mangled name>")` (an export of
`libcocos2d.dll`) and `cadence::hook`. Hook a GD function with `cadence::hook_typed`.

---

## Calling-convention and ABI pitfalls

GD is MSVC x64, so every function is `__fastcall`-compatible: `this` in `rcx`, then `rdx`,
`r8`, `r9`, then the stack. Your hook is `ReturnType __fastcall hook(void* self, ...)`.

- **`std::string` by value is a hidden pointer.** If GD declares `f(std::string s)`, your
  hook or call takes `std::string* s`.
- **Small structs returned by value use a hidden return pointer** when they have a
  constructor (for example `CCDirector::getWinSize` returns a `CCSize` through `rdx`). When in
  doubt, disassemble the caller.
- **Do not pass a 3-byte struct (`ccColor3B`) by value.** MSVC passes it in a way that is not
  safe to reproduce from a function-pointer typedef. Pass the three channels as separate
  `unsigned char` arguments to your own wrapper, or avoid the call.
- **Do not call a virtual export directly on an object of a derived class** and assume it does
  what the virtual call would. Resolving `CCNodeRGBA::setColor` by name and calling it on a
  `CCLabelBMFont` ran the base-class code against the wrong layout and crashed. Prefer
  behaviour you can get without it (different bitmap fonts instead of colours) or call through
  the object's vtable.
- Fields of `bool` / `int` / pointer type are plain loads and stores. Anything with a
  constructor or destructor (`std::vector`, `std::string`, `CCArray`) must be touched through
  the game's own functions.

---

## Reading and writing game fields

The generated headers in `generated/include/gd/` list each class's fields **in the right
order but not at reliable byte offsets**. Two offsets we first took from them were wrong, and
one of those wrote into a `std::vector`. Do not compute an offset by adding up field sizes.

Offsets we use are read out of the game's machine code and live in `cadence::off`
(`sdk.h`). To get a new one:

```powershell
pip install capstone pefile
# which [reg+disp] operands does this function use?
python tools/offset_probe.py --scan 0x3000 0x3700 "PlayLayer::togglePracticeMode"
```

Find a function that obviously uses your field (a setter, a toggle, `init`), look at the
displacement it loads or stores, and confirm it in a second function. Then add it to
`cadence::off` and to `EXPECTED` in `tools/offset_probe.py`, so that
`python tools/offset_probe.py --verify` guards it after every GD update.

If your mod depends on a class layout, export `mod_layout_hashes()` (values from
`generated/layout_hashes_<ver>.json`); the loader then refuses to load the mod when the layout
changed.

Verified offsets for GD 2.2091:

| Constant | Value | Meaning |
|---|---|---|
| `off::game_manager_play_layer` | `0x230` | `GameManager::m_playLayer` |
| `off::play_layer_is_practice_mode` | `0x32E0` | `PlayLayer::m_isPracticeMode` |
| `off::ui_layer_pause_btn` | `0x238` | `UILayer::m_pauseBtn` |
| `off::ui_layer_game_layer` | `0x258` | `UILayer::m_gameLayer` |

---

## Hot reload

**F6** reloads every mod without restarting GD:

1. Each mod's `mod_unload()` runs.
2. All of that mod's hooks are removed. Hooks from other mods stay and are re-linked, so
   removing one mod from the middle of a chain never breaks the ones above it.
3. Every `mods/*.dll` is copied to `mods/.cadence-cache/` and the **copy** is loaded.
4. `mod_main()` runs again.

Because the copies are what gets loaded, the DLLs in `mods/` are never locked: rebuild and
overwrite them while the game is running, then press F6.

The old copy stays mapped until the game exits (a few KB per mod per reload). That is
deliberate: hot reload runs inside the `drawScene` hook chain, so a mod's own detours can
still be on the call stack, and UI it created may hold callbacks into its code. Unmapping it
would turn either into a crash.

---

## Debugging

- **`loader.log`** (in the GD folder): every mod load, every `address not found` warning,
  everything you pass to `cadence::log`.
- **`crash_<timestamp>.log` / `.dmp`**: written on a fatal exception. The log has the faulting
  module and offset and a stack trace; the `.dmp` opens in Visual Studio or WinDbg. Look at the
  first frame inside `GeometryDash.exe` and the mod DLL right above it: that is the call that
  went wrong.
- `address not found: X` means no entry for that signature in the address file. Either the
  string does not match the JSON key exactly, or the function has no known address for this GD
  version. Add it to `generated/addresses.local.json` (see `README.md`).
- Run the engine tests with `ctest -C Release` from `build/`. They exercise the hook engine
  (chaining, removal from any position, restore, re-hook) without launching GD.

---

## After a GD update

```powershell
python tools/sdk_update.py --old 2.2091 --new 2.2092   # re-match addresses
python tools/offset_probe.py --verify                  # re-check every cadence::off value
```

If `--verify` reports `BAD`, use `--scan` to find the new displacement and update
`cadence::off`.
