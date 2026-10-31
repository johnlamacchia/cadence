// Hide ground: press V in a level to hide the ground and mid-ground lines.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>

static bool g_hide = false;

static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_toggleGround(PlayLayer* self, bool visible) {
    cadence::original<&PlayLayer::toggleGroundVisibility>(self, g_hide ? false : visible);
}

static void __fastcall hook_toggleMG(PlayLayer* self, bool visible) {
    cadence::original<&PlayLayer::toggleMGVisibility>(self, g_hide ? false : visible);
}

static void apply(PlayLayer* pl) {
    // Call down the chain (not through our own hooks) so the new state is applied as given.
    cadence::original<&PlayLayer::toggleGroundVisibility>(pl, !g_hide);
    cadence::original<&PlayLayer::toggleMGVisibility>(pl, !g_hide);
}

static void __fastcall hook_drawScene(void* director) {
    PlayLayer* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('V')) {
        g_hide = !g_hide;
        apply(pl);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_hide) {
        g_hide = false;
        if (PlayLayer* pl = cadence::play_layer()) apply(pl);
    }
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::toggleGroundVisibility>(hook_toggleGround);
    cadence::hook<&PlayLayer::toggleMGVisibility>(hook_toggleMG);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
