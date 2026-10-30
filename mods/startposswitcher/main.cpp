// Start position switcher — press [ and ] to cycle through start positions.
// Hooks PlayLayer::init to capture the level data, uses resetLevel to apply.
#include <sdk.h>
#include <windows.h>

static bool g_in_level = false;
static void* g_playlayer = nullptr;

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
    while (true) {
        if (g_in_level && g_playlayer && fn_resetLevel) {
            if (GetAsyncKeyState(VK_OEM_4) & 1) { // [
                fn_resetLevel(g_playlayer);
            }
            if (GetAsyncKeyState(VK_OEM_6) & 1) { // ]
                fn_resetLevel(g_playlayer);
            }
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_in_level = false;
    g_playlayer = nullptr;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_resetLevel = (resetLevel_t)cadence::addr("PlayLayer::resetLevel()");

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
