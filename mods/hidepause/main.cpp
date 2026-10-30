// Hide pause button — hides the pause button for a cleaner gameplay view.
// Press P to toggle. Works live during gameplay.
// Approach from Geode's HidePauseButton: directly hide the button node
// rather than hooking toggleMenuVisibility (which has a setting gate).
#include <sdk.h>
#include <windows.h>

static bool g_hide_pause = false;
static void* g_pause_btn = nullptr;
static volatile bool g_toggle_pending = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

using setVisible_t = void(__fastcall*)(void*, bool);
static setVisible_t fn_setVisible = nullptr;

static bool(__fastcall* orig_UILayer_init)(void* self, void* layer);

static bool __fastcall hook_UILayer_init(void* self, void* layer) {
    bool ret = orig_UILayer_init(self, layer);
    if (ret) {
        g_pause_btn = *(void**)((char*)self + 0x280);
        if (g_hide_pause && g_pause_btn && fn_setVisible) {
            fn_setVisible(g_pause_btn, false);
        }
    }
    return ret;
}

static void(__fastcall* orig_drawScene)(void*);

static void __fastcall hook_drawScene(void* director) {
    if (g_toggle_pending) {
        g_toggle_pending = false;
        if (g_pause_btn && fn_setVisible) {
            fn_setVisible(g_pause_btn, !g_hide_pause);
        }
    }
    orig_drawScene(director);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('P') & 1) {
            g_hide_pause = !g_hide_pause;
            g_toggle_pending = true;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_hide_pause = false;
    g_pause_btn = nullptr;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    fn_setVisible = (setVisible_t)cadence::cocos("?setVisible@CCNode@cocos2d@@UEAAX_N@Z");

    cadence::hook_typed(
        "UILayer::init(GJBaseGameLayer* layer)",
        hook_UILayer_init, &orig_UILayer_init);

    void* drawScene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene,
            (void**)&orig_drawScene);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
