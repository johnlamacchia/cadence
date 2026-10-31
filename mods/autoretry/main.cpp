// Auto-retry: press T in a level to toggle restarting instantly on death.
// The reset is deferred to the next frame: calling resetLevel from inside
// destroyPlayer re-enters the death logic and loops forever.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>

static bool g_autoretry = false;
static bool g_reset_pending = false;

static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_destroyPlayer(PlayLayer* self, PlayerObject* player, GameObject* object) {
    cadence::original<&PlayLayer::destroyPlayer>(self, player, object);
    if (g_autoretry) g_reset_pending = true;
}

static void __fastcall hook_drawScene(void* director) {
    PlayLayer* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('T')) g_autoretry = !g_autoretry;

    if (g_reset_pending) {
        g_reset_pending = false;
        if (pl) {
            if (auto reset = cadence::fn<&PlayLayer::resetLevel>()) reset(pl);
        }
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_autoretry = false;
    g_reset_pending = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::destroyPlayer>(hook_destroyPlayer);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
