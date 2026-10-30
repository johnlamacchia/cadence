// Hide ground — press V in a level to hide the ground and mid-ground lines.
#include <sdk.h>

static bool g_hide = false;

static void(__fastcall* orig_toggleGround)(void* self, bool visible);
static void(__fastcall* orig_toggleMG)(void* self, bool visible);
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_toggleGround(void* self, bool visible) {
    orig_toggleGround(self, g_hide ? false : visible);
}

static void __fastcall hook_toggleMG(void* self, bool visible) {
    orig_toggleMG(self, g_hide ? false : visible);
}

static void __fastcall hook_drawScene(void* director) {
    void* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('V')) {
        g_hide = !g_hide;
        // Call the trampolines so our own hooks don't override the new state.
        orig_toggleGround(pl, !g_hide);
        orig_toggleMG(pl, !g_hide);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_hide) {
        g_hide = false;
        void* pl = cadence::play_layer();
        if (pl) {
            orig_toggleGround(pl, true);
            orig_toggleMG(pl, true);
        }
    }
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hide Ground","desc":"Hides ground and mid-ground lines","key":"V","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed("PlayLayer::toggleGroundVisibility(bool visible)", hook_toggleGround, &orig_toggleGround);
    cadence::hook_typed("PlayLayer::toggleMGVisibility(bool visible)", hook_toggleMG, &orig_toggleMG);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
