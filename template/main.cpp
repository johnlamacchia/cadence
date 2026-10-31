// MY_MOD: starter mod. Press K in a level to log how many times it was reset.
//
// Build:  cmake -B build -A x64 -DCADENCE_SDK_DIR=<path to cadence> && cmake --build build --config Release
// Run:    copy build/Release/MY_MOD.dll into <Geometry Dash>/mods/ and press F6 in game.
#include <sdk_extras.h>      // sdk.h plus play_layer() and key_pressed(); sdk.h alone is the whole API
#include <gd/PlayLayer.hpp>  // the class you hook: declarations, members, hook addresses

static int g_resets = 0;

// 1. A hook. The signature comes from the declaration in gd/PlayLayer.hpp, so a
//    wrong one is a compile error. `self` is a real PlayLayer*: read its members directly.
static void __fastcall hook_resetLevel(PlayLayer* self) {
    g_resets++;
    cadence::original<&PlayLayer::resetLevel>(self);   // call through unless you mean to replace it
}

// 2. Per-frame work belongs in a hook that runs on the game thread. drawScene runs
//    once per frame. It is a cocos2d function (no GD address), so hook it by name.
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_drawScene(void* director) {
    // play_layer() is the live level (nullptr in menus). Never cache it yourself.
    if (PlayLayer* layer = cadence::play_layer()) {
        if (cadence::key_pressed('K'))
            cadence::log("MY_MOD: reset %d times; practice mode is %s", g_resets, layer->m_isPracticeMode ? "on" : "off");
    }
    orig_drawScene(director);
}

// 3. Optional: undo anything the loader cannot. Hooks are removed for you.
extern "C" __declspec(dllexport) void mod_unload() {
    g_resets = 0;
}

// 4. Required. (The name, author, etc. shown by the mod menu come from CMakeLists.txt.)
extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook<&PlayLayer::resetLevel>(hook_resetLevel);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
