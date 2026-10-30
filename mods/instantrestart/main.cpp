// Instant restart — press R while playing a level to restart instantly.
// Hooks UILayer::keyDown to intercept the R key.
#include <sdk.h>
#include <windows.h>

static void* g_playLayer = nullptr;

// PlayLayer::init — track the current PlayLayer instance
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    bool result = orig_PlayLayer_init(self, level, dontCreate);
    if (result) g_playLayer = self;
    return result;
}

// PlayLayer::onQuit — clear the instance
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_playLayer = nullptr;
    orig_PlayLayer_onQuit(self);
}

// UILayer::keyDown — intercept R key
// Signature: void UILayer::keyDown(cocos2d::enumKeyCodes key, double timestamp)
// enumKeyCodes for 'R' is 82 (same as ASCII)
static void(__fastcall* orig_UILayer_keyDown)(void* self, int key, double timestamp);

static void __fastcall hook_UILayer_keyDown(void* self, int key, double timestamp) {
    if (key == 'R' && g_playLayer) {
        // Call PlayLayer::resetLevel()
        using resetLevel_t = void(__fastcall*)(void*);
        auto resetLevel = (resetLevel_t)cadence::addr("PlayLayer::resetLevel()");
        if (resetLevel) {
            resetLevel(g_playLayer);
            return;
        }
    }
    orig_UILayer_keyDown(self, key, timestamp);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_playLayer = nullptr;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    cadence::hook_typed(
        "UILayer::keyDown(cocos2d::enumKeyCodes key, double timestamp)",
        hook_UILayer_keyDown, &orig_UILayer_keyDown);
}
