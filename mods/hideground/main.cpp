// Hide ground — removes ground and middle-ground lines for a clean gameplay view.
// Press V to toggle ground visibility.
#include <sdk.h>
#include <windows.h>

static bool g_hide = true;

static void(__fastcall* orig_toggleGroundVisibility)(void* self, bool visible);
static void(__fastcall* orig_toggleMGVisibility)(void* self, bool visible);

static void __fastcall hook_toggleGroundVisibility(void* self, bool visible) {
    if (g_hide) {
        orig_toggleGroundVisibility(self, false);
        return;
    }
    orig_toggleGroundVisibility(self, visible);
}

static void __fastcall hook_toggleMGVisibility(void* self, bool visible) {
    if (g_hide) {
        orig_toggleMGVisibility(self, false);
        return;
    }
    orig_toggleMGVisibility(self, visible);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (true) {
        if (GetAsyncKeyState('V') & 1) {
            g_hide = !g_hide;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_hide = true;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::toggleGroundVisibility(bool visible)",
        hook_toggleGroundVisibility, &orig_toggleGroundVisibility);

    cadence::hook_typed(
        "PlayLayer::toggleMGVisibility(bool visible)",
        hook_toggleMGVisibility, &orig_toggleMGVisibility);

    CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
