# Cadence mod guide

Everything you need to write a mod that does not crash the game. The API itself is
small (`loader/include/sdk.h`, about 250 lines with comments); most of this page is
about how Geometry Dash behaves, because that is where mods go wrong.

- [Your first mod](#your-first-mod)
- [The typed API](#the-typed-api)
- [The rules](#the-rules)
- [Recipes](#recipes)
- [Calling-convention and ABI pitfalls](#calling-convention-and-abi-pitfalls)
- [Class layouts and how far to trust them](#class-layouts-and-how-far-to-trust-them)
- [Hot reload](#hot-reload)
- [Debugging](#debugging)
- [After a GD update](#after-a-gd-update)

---

## Your first mod

**With the release zip** (the usual way): unzip it, then from any folder you want your project in:

```powershell
python <unzipped>\sdk\tools\new_mod.py my_mod       # a complete standalone project in .\my_mod\
cd my_mod
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
copy build\Release\my_mod.dll "<Geometry Dash>\mods\"
```

The project it makes has the whole `CMakeLists.txt` (it points at the SDK it was made from; override with
`-DCADENCE_SDK_DIR=<path>` if you move it), a starter `main.cpp` and a `.gitignore`. Options:
`--author`, `--description`, `--key`, `--display-name`.

**Inside a repo checkout** the same script makes a mod the root build finds by itself:

```powershell
python tools/new_mod.py my_mod                 # creates mods/my_mod/
cmake -B build -A x64
cmake --build build --config Release --target my_mod_mod
copy build\mods\my_mod\Release\my_mod.dll "<Geometry Dash>\mods\"
```

Press **F6** in game to load it. After that, edit, rebuild, copy the DLL over the old one
(it is not locked, see [Hot reload](#hot-reload)) and press F6 again.

You can also copy `template/` by hand, rename `MY_MOD`, and configure with `-DCADENCE_SDK_DIR=<path>`.

A complete mod:

```cpp
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static void __fastcall my_reset(PlayLayer* self) {
    cadence::log("level reset, practice mode: %d", self->m_isPracticeMode);
    cadence::original<&PlayLayer::resetLevel>(self);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::resetLevel>(my_reset);
}
```

The mod's name, author, description, key and version are not in the code. They are arguments
to `cadence_add_mod()` in its `CMakeLists.txt` and are baked into the DLL:

```cmake
cadence_add_mod(my_mod main.cpp NAME "My Mod" AUTHOR "me" DESCRIPTION "Does a thing" KEY "M" VERSION "1.0")
```

---

## The typed API

`generated/include/gd/<Class>.hpp` declares each GD class: its bases, its member functions and
its data members, with `static_assert`s on `sizeof` and every member's offset. Include the
headers for the classes you use and the compiler checks your hooks against the real signatures.

```cpp
cadence::hook<&PlayLayer::resetLevel>(detour);     // detour must be void(__fastcall*)(PlayLayer*)
cadence::original<&PlayLayer::resetLevel>(self);   // the next function in the chain (typed)
auto f = cadence::fn<&PlayLayer::resetLevel>();    // the game's own function, to call it (nullptr if unknown)
```

- **Wrong signature = compile error.** The detour type is derived from the declaration.
- **A function with no Windows address cannot be hooked,** and says so at compile time: the
  bindings mark it `win inline` or give it no `win` address (`MenuLayer::onMoreGames` is one
  in 2.2091). `cadence::hook_typed("Class::fn(args)", detour, &orig)` takes the signature as a
  string instead and just returns `false` (with a warning in `loader.log`) if it has no address.
- **Overloads** need the usual cast:
  `cadence::hook<static_cast<bool (Foo::*)(int)>(&Foo::bar)>(detour)`. Static functions the same:
  `cadence::fn<static_cast<Foo* (*)(int)>(&Foo::create)>()`.
- **One hook per function per mod.** `original<F>` is one variable per F, so a second hook on the
  same function in the same DLL points the first detour's "original" back at itself and recurses
  until the stack is gone (this crashed a mod before the main menu). If two features need the
  same function, hook it once and call both from the one detour.
- **Pass a member-function pointer to `hook`/`original`/`fn` as an expression,** not through a
  `constexpr` variable: MSVC did not treat `constexpr auto k = static_cast<...>(&C::f)` as the
  same template argument, so `fn_sig<k>` was not found. Write the `static_cast` in the template
  argument, or wrap it in a macro.
- A function that returns a struct through a hidden pointer cannot be hooked with `hook<F>`
  (its compile-time check explains why); use `cadence::hook(addr, detour, &orig)` and declare the
  detour with the return slot first.
- `cadence::fn<F>()` on a **virtual** function calls that class's implementation directly,
  not through the vtable.

Things the headers do not do: they define no function bodies, so you cannot call a GD member
function directly (it would not link). Use `cadence::fn<>()`, or a virtual call on an object, where
the vtable layout is the bindings' (see [below](#class-layouts-and-how-far-to-trust-them)).

Optional helpers live in `sdk_extras.h`: `cadence::play_layer()`, `cadence::key_pressed()`, and a small set for switching
mods on and off (`disabled_mods()`, `write_disabled_mods()`, `reload_mods()`, `loader_version()`, `game_folder()`).
The loader and `sdk.h` do not depend on them.

**Switching mods off.** The loader does not run a mod listed in `cadence_disabled.txt` next to `GeometryDash.exe` (one file
name per line, e.g. `hitboxes.dll`; `#` starts a comment, case does not matter). A disabled mod still appears in
`api->get_mods` with `enabled = false` and its name, author and description, so a menu can offer to switch it back on; its
`mod_main` is never called. `cadence::reload_mods()` makes the loader re-read the file (same as F6). The `modmenu` example is
the working version of this. Both features need loader version 2; on an older loader `loader_version()` returns 1 and
`reload_mods()` returns false, so check before relying on them. (The version and reload are exported by name from the loader,
`cadence_loader_version` and `cadence_reload_mods`, not added to the `LoaderAPI` table, so a mod built against the old header
keeps working.) A mod must not disable itself.

### GD's own UI: `sdk_ui.h`

Also optional. Popups, buttons and checkboxes built from the game's own classes, with no cocos2d
subclassing:

```cpp
#include <sdk_ui.h>

cadence::ui::alert("Hello", "A GD popup.");
cadence::ui::confirm("Sure?", "Pick one.", "No", "Yes", [](bool yes) { /* ... */ });

cadence::ui::Popup("My mod", 300, 180)
    .label("Options", 0, 55)
    .checkbox("Show hitboxes", -100, 20, on, [](bool v) { on = v; })
    .button("Reset", 0, -50, [] { reset(); })
    .show();
```

- Coordinates are relative to the **centre of the popup** (GD's design height is 320). Where you put
  things, and how much room you leave, is up to you; read `popup.width()` / `popup.height()` for the
  window the game actually gave you.
- By default the game sizes the window to its (empty) text and ignores the height you ask for.
  `Popup(title, w, h, /*tall=*/true)` makes the height real. Lay out from `width()` / `height()`.
- `button()` takes `scale`, `height`, a background sprite and an optional fixed `width`;
  `icon_button()` takes any sprite frame name; `label()` and `checkbox()` take a maximum width and
  shrink long text to fit.
- To change what a popup shows, `clear()` it and add again, **from per-frame code**, not from inside
  one of its own button callbacks (a button must not delete itself while it handles its click).
  Set a flag in the callback and rebuild on the next frame.
- `close()` dismisses it; `is_open()` tells you whether it still exists.
- Callbacks run on the game thread. `mods/uidemo` (F7) exercises all of it.

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
// mod_main (a cocos2d export has no bindings address, so look it up by name):
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
#include <sdk_extras.h>
PlayLayer* pl = cadence::play_layer();   // nullptr outside a level, never stale
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
on the next frame (see `mods/autoretry`).

### 4. Never block a hook

`Sleep()` in a hook that runs on the game thread freezes the whole game. For timers, record
`QueryPerformanceCounter` and compare it on later frames (see `mods/pausecountdown`).

### 5. Poll keys with `cadence::key_pressed`

```cpp
if (cadence::play_layer() && cadence::key_pressed('N')) { ... }
```

`key_pressed` fires once per physical press, ignores input while GD is not the foreground
window, and needs no thread. Call it at most once per key per frame from a game-thread hook.
Do not use `GetAsyncKeyState(key) & 1`: the low bit is not reliable and it fires when GD is
in the background. Gate toggles on `play_layer()` if the key could also be typed into a text
field (level search, comments).

### 6. Always call the original (unless you mean not to)

Skipping `original<F>` replaces the function. That is the point of a noclip mod's
`destroyPlayer` hook, but it is a bug in almost every other hook. Hooks from several mods
chain automatically, and a mod that does not call through breaks every mod hooked before it.

### 7. Own your state in `mod_unload`

The loader removes your hooks on reload. It cannot know what else you changed. Anything you
altered in the game (hidden nodes, flags you flipped, the window title) belongs in
`mod_unload`, so a reload leaves the game as you found it.

---

## Recipes

Each of these exists as a working mod in `mods/`.

| I want to... | Do this | Example |
|---|---|---|
| Change what a function returns | Hook it, return your value, optionally call `original` | `hitboxes`, `anticheat` |
| Stop something happening | Hook it and `return` without calling `original` | `noclip` |
| Change an argument | Call `original` with a different value | `hideground`, `noglow` |
| React to something happening | Call `original`, then run your code | `attemptcounter` |
| Read or write a game field | `self->m_isPracticeMode` on the typed `self` | `practicemusic`, `hidepause` |
| Call a game function | `cadence::fn<&Class::function>()` | `example`, `instantrestart` |
| Run every frame | Hook `CCDirector::drawScene` or `CCScheduler::update` | `autoretry`, `speedhack` |
| Respond to a key | `cadence::key_pressed` in a `drawScene` hook, or hook `UILayer::keyDown` | `noclip`, `instantrestart` |
| Show UI | `CCLabelBMFont::create`, `addChild` on the running scene, from the game thread | `fpsdisplay`, `pausecountdown`, `modmenu` |
| Read what other mods are loaded | `cadence::get_mods` | `modmenu` |

---

## Calling-convention and ABI pitfalls

GD is MSVC x64, so every function is `__fastcall`-compatible: `this` in `rcx`, then `rdx`,
`r8`, `r9`, then the stack. A detour is `Ret __fastcall hook(Class* self, ...)`.

- **Match the return type exactly.** Our own `anticheat` hook was written `void` for a function
  that returns `bool`: it returned whatever was left in `rax`. The typed API turns that into a
  compile error.
- **`std::string` by value is a hidden pointer.** The detour's `std::string` parameter handles it.
- **Small structs returned by value use a hidden return pointer** when they have a
  constructor (for example `CCDirector::getWinSize` returns a `CCSize` through `rdx`).
- **`CCPoint`, `CCSize` and `CCRect` are passed by hidden reference, not in registers,** even
  though they are only 8 or 16 bytes: the game's versions have copy constructors, which makes
  MSVC treat them as non-trivial. The generated headers declare them the same way, so the typed
  API gets it right. If you declare your own function type that takes one by value, give your
  struct a copy constructor too, or you will pass the two floats where the game reads a pointer
  (this crashed inside libcocos2d the first time we called `playPlatformerEndAnimationToPos`).
- **A method inherited from a second base class gets that base's `this`.** `FLAlertLayer`'s
  `keyBackClicked` belongs to a base class embedded 0x150 bytes into the layer, and the function
  starts by subtracting 0x150 to get back to the layer. Called with the layer pointer it computes
  garbage and crashes. The headers list such methods under the derived class, so the typed
  `self` of a hook on one is the adjusted pointer, not the object. When a call or hook on an
  inherited virtual misbehaves, look at the function's first instructions for a `lea reg, [rcx - N]`.
- **Do not pass a 3-byte struct (`ccColor3B`) by value** to a function you declare yourself.
  MSVC passes it in a way that is not safe to reproduce from a function-pointer typedef.
- **Do not call a virtual cocos2d export directly on an object of a derived class** and assume
  it does what the virtual call would. Resolving `CCNodeRGBA::setColor` by name and calling it on
  a `CCLabelBMFont` ran the base-class code against the wrong layout and crashed.
- Fields of `bool` / `int` / pointer type are plain loads and stores. Anything with a
  constructor or destructor (`std::vector`, `std::string`, `CCArray`) must be touched through
  the game's own functions.
- Build in **Release**. The headers refuse to compile with the debug STL
  (`_ITERATOR_DEBUG_LEVEL != 0`), whose containers have different sizes from the game's.

---

## Class layouts and how far to trust them

The generator computes each class's layout from the member lists in the Broma bindings using
the MSVC x64 object model, then emits `static_assert`s for it. The compiler checks them: if our
model of MSVC and the compiler's disagree for any class or member, the generated headers do not
build. Whether the *bindings* describe the real class correctly is a separate question, and we
check that against the game binary:

| Status in `generated/layout_report_<ver>.json` | Classes (2.2091) | Meaning |
|---|---|---|
| `verified` | 351 | The class's `sizeof` read out of `GeometryDash.exe` equals the size computed from the bindings. |
| `tail-padded` | 62 | The binary's class is **larger** than the bindings describe: members Geode never mapped. Every mapped member's offset is still computed from the front, and an explicit `_unmapped_tail` pad makes `sizeof` right. Anything in the tail is invisible to you. |
| `corrected` | 3 | The bindings describe **more** than the binary has (`CCSet`, `UndoObject`, `MusicDownloadManager`): members that would end past the real size cannot exist there, so the generator drops them (listed as `dropped_members`) and pads to the binary's size. |
| `known-short` | 35 | No binary size is known, but the game's own code uses memory past where the bindings say the class ends: it has members the bindings never named, so `sizeof` in the header is too small (`GJGameLevel` is the big one). Members near the front are still at the right offsets. None of these is a base class of another class, so no other class's offsets are shifted by the gap. |
| `unverified` | 653 | Computed from the bindings; there is no size in the binary to compare against. See the offset audit below for how much evidence there is for their offsets anyway. |
| `incomplete` / `not generated` | 6 / 10 | A type the bindings use is not defined (or it is FMOD, the audio library). No header; the class is only forward-declared. |

**Where the binary sizes come from** (`tools/layout_check.py harvest` and `tools/rtti_sizes.py`), two
independent places that must agree, and the generator refuses to run if they ever do not:

- `X::create` starts with `mov ecx, sizeof(X); call operator new`.
- The class's vtable holds a deleting destructor ending in `mov edx, sizeof(X); call operator delete`.
  The vtables are found through the exe's RTTI.

On the 88 classes both cover, they agree on all 88. Two more ideas were tried and rejected because they
failed that same check: scanning more than the first four vtable slots (it picks up unrelated
functions and returns wrong sizes) and reading the size from `new` + constructor call sites (a derived
class's constructor calls its base's, so the base is reported with the derived size).

**The offset audit** (`tools/offset_audit.py`) is a second test, because a class can have the right size
and still have a member at the wrong offset. It reads the machine code of every member function with a
Windows address, follows `this`, and checks each `[this + offset]` access against the layout. On 2.2091
it traced **21,656 accesses that agree with the layouts and 17 that do not** (0.08%), across 536 classes.
The 17 are in 10 classes and all trace back to placeholder members the bindings leave as one wide field
(for example `GJBaseGameLayer::m_unk7b0`, which the game reads byte by byte); no named member was found
at a wrong offset. This finds contradictions; it cannot prove a layout right.

Also confirmed by hand from machine code: the offsets `tools/layout_check.py check` lists (for example
`GJBaseGameLayer::m_isPracticeMode` at `0x32E0` and `m_debugDrawNode` at `0x3870`).

**Virtual calls through the headers** (`node->setVisible(false)`) dispatch through the vtable,
so they rely on the bindings listing virtual functions in the game's vtable order, as Geode
does. We have not verified vtable order class by class, so the example mods call cocos2d through
`GetProcAddress`'d exports as before, and hooks use addresses, which do not depend on it.

If your mod depends on a class layout, you are already covered: each class header registers its
layout hash, the mod exports them, and the loader **refuses to load the mod, naming the class**,
if the game's layout hash differs. After a GD update, a mod built against the old layouts will not
run with the new ones until rebuilt.

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
  every refused mod, everything you pass to `cadence::log`.
- **`crash_<timestamp>.log` / `.dmp`**: written on a fatal exception. The log has the faulting
  module and offset and a stack trace; the `.dmp` opens in Visual Studio or WinDbg. Look at the
  first frame inside `GeometryDash.exe` and the mod DLL right above it: that is the call that
  went wrong.
- `address not found: X` means no entry for that signature in the address file. Either the
  string does not match the JSON key exactly, or the function has no known address for this GD
  version. Add it to `generated/addresses.local.json` (see `README.md`).
- Addresses must point into `GeometryDash.exe`'s code; the loader ignores any that do not (and
  says so in `loader.log`), so a typo in a local override cannot corrupt the game.
- `ctest -C Release` from `build/` runs the hook-engine tests and a loader smoke test that loads
  every mod without launching GD.

---

## After a GD update

```powershell
python tools/sdk_update.py --old 2.2091 --new 2.2092   # re-match addresses, regenerate, report
python tools/layout_check.py check <bindings> --version 2.2092
```

`sdk_update` lists the classes whose `sizeof` changed in the new exe and the functions it could
not place. Rebuild your mods afterwards: a mod using a class whose layout changed is refused
until it is rebuilt against the new headers.
