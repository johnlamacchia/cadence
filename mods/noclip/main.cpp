// Noclip — press N in a level to toggle surviving every hazard.
#include <sdk.h>

static bool g_noclip = false;

static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);
static void(__fastcall* orig_drawScene)(void* director);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    if (g_noclip) return;
    orig_destroyPlayer(self, player, object);
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::play_layer() && cadence::key_pressed('N')) g_noclip = !g_noclip;
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() { g_noclip = false; }

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Noclip","desc":"Prevents player death","key":"N","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed("PlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)",
                        hook_destroyPlayer, &orig_destroyPlayer);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
