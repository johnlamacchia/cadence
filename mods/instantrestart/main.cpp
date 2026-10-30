// Instant restart — press R in a level to restart it immediately.
// UILayer::keyDown only fires for keys GD itself dispatches, so this works
// with GD's own key handling and no polling.
#include <sdk.h>

static void(__fastcall* fn_resetLevel)(void* self) = nullptr;
static void(__fastcall* orig_UILayer_keyDown)(void* self, int key, double timestamp);

static void __fastcall hook_UILayer_keyDown(void* self, int key, double timestamp) {
    void* pl = cadence::play_layer();
    if (key == 'R' && pl && fn_resetLevel) {
        fn_resetLevel(pl);
        return;
    }
    orig_UILayer_keyDown(self, key, timestamp);
}

extern "C" __declspec(dllexport) void mod_unload() {}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Instant Restart","desc":"Press R to restart the level","key":"R","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_resetLevel = (decltype(fn_resetLevel))cadence::addr("PlayLayer::resetLevel()");

    cadence::hook_typed("UILayer::keyDown(cocos2d::enumKeyCodes key, double timestamp)",
                        hook_UILayer_keyDown, &orig_UILayer_keyDown);
}
