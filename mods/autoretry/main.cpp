// Auto-retry — automatically restarts the level on death, no death screen.
// Press T to toggle on/off. Reset is deferred to next frame to avoid
// recursive death loops.
#include <sdk.h>
#include <windows.h>

static bool g_autoretry = false;
static volatile bool g_in_level = false;
static volatile bool g_reset_pending = false;
static void* g_playlayer = nullptr;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

using resetLevel_t = void(__fastcall*)(void* self);
static resetLevel_t fn_resetLevel = nullptr;

static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);
static void(__fastcall* orig_scheduler_update)(void*, float);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    orig_destroyPlayer(self, player, object);
    if (g_autoretry && g_in_level) {
        g_playlayer = self;
        g_reset_pending = true;
    }
}

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    bool ret = orig_PlayLayer_init(self, level, dontCreate);
    g_playlayer = self;
    g_in_level = true;
    g_reset_pending = false;
    return ret;
}

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_in_level = false;
    g_reset_pending = false;
    g_playlayer = nullptr;
    orig_PlayLayer_onQuit(self);
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (g_reset_pending && g_in_level && g_playlayer && fn_resetLevel) {
        g_reset_pending = false;
        fn_resetLevel(g_playlayer);
    }
}

static DWORD WINAPI key_thread(LPVOID) {
    bool was_down = false;
    while (g_running) {
        bool is_down = (GetAsyncKeyState('T') & 0x8000) != 0;
        if (is_down && !was_down) {
            g_autoretry = !g_autoretry;
        }
        was_down = is_down;
        Sleep(30);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_in_level = false;
    g_reset_pending = false;
    g_playlayer = nullptr;
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

    void* schedUpdate = cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update,
            (void**)&orig_scheduler_update);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
