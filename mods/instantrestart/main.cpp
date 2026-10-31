// Instant restart: press R in a level to restart it immediately.
// Hooks UILayer::keyDown, so it sees exactly the keys GD itself dispatches.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>
#include <gd/UILayer.hpp>

static void __fastcall hook_keyDown(UILayer* self, cocos2d::enumKeyCodes key, double timestamp) {
    PlayLayer* pl = cadence::play_layer();
    if (pl && static_cast<int>(key) == 'R') {
        if (auto reset = cadence::fn<&PlayLayer::resetLevel>()) {
            reset(pl);
            return;
        }
    }
    cadence::original<&UILayer::keyDown>(self, key, timestamp);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&UILayer::keyDown>(hook_keyDown);
}
