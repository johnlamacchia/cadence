// MY_MOD — starter mod. Press K in a level to log how many times it was reset.
//
// Build:  cmake -B build -A x64 -DCADENCE_SDK_DIR=<path to cadence> && cmake --build build --config Release
// Run:    copy build/Release/MY_MOD.dll into <Geometry Dash>/mods/ and press F6 in game.
#include <sdk.h>

static int g_resets = 0;

// 1. A hook: same signature as the game function, __fastcall, `this` first.
static void(__fastcall* orig_resetLevel)(void* self);

static void __fastcall hook_resetLevel(void* self) {
    g_resets++;
    orig_resetLevel(self);  // always call through unless you mean to replace the function
}

// 2. Per-frame work belongs in a hook that runs on the game thread. drawScene
//    runs once per frame; key_pressed() must be polled from there, not from a thread.
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_drawScene(void* director) {
    // play_layer() is the live level (nullptr in menus). Never cache it yourself.
    if (cadence::play_layer() && cadence::key_pressed('K'))
        cadence::log("MY_MOD: level reset %d times this session", g_resets);

    orig_drawScene(director);
}

// 3. Optional: shown by the Mod Menu.
extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"MY_MOD","author":"you","desc":"What it does","key":"K","ver":"1.0"})";
}

// 4. Optional: undo anything the loader can't. Hooks are removed for you.
extern "C" __declspec(dllexport) void mod_unload() {
    g_resets = 0;
}

// 5. Required.
extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed("PlayLayer::resetLevel()", hook_resetLevel, &orig_resetLevel);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
