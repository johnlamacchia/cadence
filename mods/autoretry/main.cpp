// Auto-retry — automatically restarts the level on death, no death screen.
// Press T to toggle on/off.
#include <sdk.h>
#include <windows.h>

static bool g_autoretry = false;
static bool g_in_level = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

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
    while (g_running) {
        if (GetAsyncKeyState('T') & 1) {
            g_autoretry = !g_autoretry;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_in_level = false;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Auto Retry","desc":"Auto-restart on death","key":"T","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

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

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
