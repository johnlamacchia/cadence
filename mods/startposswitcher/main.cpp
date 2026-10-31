// Start pos switcher: press [ or ] in a level to restart it.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>

static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_drawScene(void* director) {
    PlayLayer* pl = cadence::play_layer();
    bool left = cadence::key_pressed(VK_OEM_4);
    bool right = cadence::key_pressed(VK_OEM_6);
    if (pl && (left || right)) {
        if (auto reset = cadence::fn<&PlayLayer::resetLevel>()) reset(pl);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
