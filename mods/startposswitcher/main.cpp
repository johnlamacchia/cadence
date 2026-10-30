// Start pos switcher — press [ or ] in a level to restart it.
#include <sdk.h>

static void(__fastcall* fn_resetLevel)(void* self) = nullptr;
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_drawScene(void* director) {
    void* pl = cadence::play_layer();
    bool left = cadence::key_pressed(VK_OEM_4);
    bool right = cadence::key_pressed(VK_OEM_6);
    if (pl && (left || right) && fn_resetLevel) fn_resetLevel(pl);
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Start Pos Switcher","desc":"Quick level reset","key":"[ / ]","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_resetLevel = (decltype(fn_resetLevel))cadence::addr("PlayLayer::resetLevel()");

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
