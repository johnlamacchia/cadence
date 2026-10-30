// Hide pause button — removes the pause button UI for a cleaner gameplay view.
// Press P to toggle visibility (works live during gameplay).
#include <sdk.h>
#include <windows.h>

static bool g_hide_pause = false;
static void* g_uilayer = nullptr;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

static void(__fastcall* orig_toggleMenuVisibility)(void* self, bool visible);

static void __fastcall hook_toggleMenuVisibility(void* self, bool visible) {
    g_uilayer = self;
    if (g_hide_pause) {
        orig_toggleMenuVisibility(self, false);
        return;
    }
    orig_toggleMenuVisibility(self, visible);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('P') & 1) {
            g_hide_pause = !g_hide_pause;
            if (g_uilayer) {
                orig_toggleMenuVisibility(g_uilayer, !g_hide_pause);
            }
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_hide_pause = false;
    g_uilayer = nullptr;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    cadence::hook_typed(
        "UILayer::toggleMenuVisibility(bool visible)",
        hook_toggleMenuVisibility, &orig_toggleMenuVisibility);

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
