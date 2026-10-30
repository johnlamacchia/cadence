// Auto-retry — automatically restarts the level on death, no death screen.
// Press T to toggle on/off.
#include <sdk.h>
#include <windows.h>

static bool g_autoretry = true;
static bool g_in_level = false;

using resetLevel_t = void(__fastcall*)(void* self);
static resetLevel_t fn_resetLevel = nullptr;

static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    orig_destroyPlayer(self, player, object);
    if (g_autoretry && fn_resetLevel) {
        fn_resetLevel(self);
    }
}

static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    g_in_level = true;
    return orig_PlayLayer_init(self, level, dontCreate);
}

static void(__fastcall* orig_PlayLayer_onQuit)(void* self);

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_in_level = false;
    orig_PlayLayer_onQuit(self);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (true) {
        if (GetAsyncKeyState('T') & 1) {
            g_autoretry = !g_autoretry;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_in_level = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_resetLevel = (resetLevel_t)cadence::addr("PlayLayer::resetLevel()");

    cadence::hook_typed(
        "PlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)",
        hook_destroyPlayer, &orig_destroyPlayer);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
