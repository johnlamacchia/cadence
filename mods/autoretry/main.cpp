// Auto-retry — press T in a level to toggle restarting instantly on death.
// The reset is deferred to the next frame: calling resetLevel from inside
// destroyPlayer re-enters the death logic and loops forever.
#include <sdk.h>

static bool g_autoretry = false;
static bool g_reset_pending = false;

static void(__fastcall* fn_resetLevel)(void* self) = nullptr;
static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    orig_destroyPlayer(self, player, object);
    if (g_autoretry) g_reset_pending = true;
}

static void __fastcall hook_drawScene(void* director) {
    void* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('T')) g_autoretry = !g_autoretry;

    if (g_reset_pending) {
        g_reset_pending = false;
        if (pl && fn_resetLevel) fn_resetLevel(pl);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_autoretry = false;
    g_reset_pending = false;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Auto Retry","desc":"Auto-restart on death","key":"T","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_resetLevel = (decltype(fn_resetLevel))cadence::addr("PlayLayer::resetLevel()");

    cadence::hook_typed("PlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)",
                        hook_destroyPlayer, &orig_destroyPlayer);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
