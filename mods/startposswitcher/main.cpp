// Start position switcher — press [ and ] to quick-reset the level.
#include <sdk.h>
#include <windows.h>

static bool g_in_level = false;
static void* g_playlayer = nullptr;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

using resetLevel_t = void(__fastcall*)(void* self);
static resetLevel_t fn_resetLevel = nullptr;

static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    bool ret = orig_PlayLayer_init(self, level, dontCreate);
    if (ret) {
        g_in_level = true;
        g_playlayer = self;
    }
    return ret;
}

static void(__fastcall* orig_PlayLayer_onQuit)(void* self);

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_in_level = false;
    g_playlayer = nullptr;
    orig_PlayLayer_onQuit(self);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (g_in_level && g_playlayer && fn_resetLevel) {
            if (GetAsyncKeyState(VK_OEM_4) & 1) {
                fn_resetLevel(g_playlayer);
            }
            if (GetAsyncKeyState(VK_OEM_6) & 1) {
                fn_resetLevel(g_playlayer);
            }
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_in_level = false;
    g_playlayer = nullptr;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Start Pos Switcher","desc":"Quick level reset","key":"[ / ]","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    fn_resetLevel = (resetLevel_t)cadence::addr("PlayLayer::resetLevel()");

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
