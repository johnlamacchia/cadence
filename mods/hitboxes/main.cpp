// Hitboxes: press H in a level to toggle the game's own debug hitbox drawing.
//
// PlayLayer::updateVisibility already redraws the hitboxes every frame while
// shouldDrawDebugHitboxes() is true, so all we do is answer "yes" and ask the
// game to sync the draw node's visibility once when the key is pressed.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>

static bool g_show = false;

static void(__fastcall* orig_drawScene)(void* director);

static bool __fastcall hook_playlayer_should_draw(PlayLayer* self) {
    return g_show || cadence::original<&PlayLayer::shouldDrawDebugHitboxes>(self);
}

static bool __fastcall hook_base_should_draw(GJBaseGameLayer* self) {
    return g_show || cadence::original<&GJBaseGameLayer::shouldDrawDebugHitboxes>(self);
}

// Ask the game to show or hide the hitbox draw node to match shouldDrawDebugHitboxes().
static void sync(PlayLayer* pl) {
    if (auto toggle = cadence::fn<&PlayLayer::toggleDebugDraw>()) toggle(pl);
}

static void __fastcall hook_drawScene(void* director) {
    PlayLayer* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('H')) {
        g_show = !g_show;
        sync(pl);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    // Hide the hitboxes we turned on; the hooks are removed by the loader.
    if (g_show) {
        g_show = false;
        if (PlayLayer* pl = cadence::play_layer()) sync(pl);
    }
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::shouldDrawDebugHitboxes>(hook_playlayer_should_draw);
    cadence::hook<&GJBaseGameLayer::shouldDrawDebugHitboxes>(hook_base_should_draw);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
