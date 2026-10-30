// Noclip mod — press N to toggle noclip (prevents player death).
#include <sdk.h>
#include <windows.h>

static bool g_noclip = false;
static bool g_in_level = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    if (g_noclip) return;
    orig_destroyPlayer(self, player, object);
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
        if (GetAsyncKeyState('N') & 1) {
            g_noclip = !g_noclip;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_in_level = false;
    g_noclip = false;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Noclip","desc":"Prevents player death","key":"N","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

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
