// Noclip: press N in a level to toggle surviving every hazard.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>

static bool g_noclip = false;

static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_destroyPlayer(PlayLayer* self, PlayerObject* player, GameObject* object) {
    if (g_noclip) return;
    cadence::original<&PlayLayer::destroyPlayer>(self, player, object);
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::play_layer() && cadence::key_pressed('N')) g_noclip = !g_noclip;
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_noclip = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::destroyPlayer>(hook_destroyPlayer);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
