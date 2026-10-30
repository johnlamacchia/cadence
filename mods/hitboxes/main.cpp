// Hitbox viewer — press H to toggle debug hitbox rendering.
#include <sdk.h>
#include <windows.h>

static bool g_show_hitboxes = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

static bool(__fastcall* orig_shouldDraw)(void* self);

static bool __fastcall hook_shouldDraw(void* self) {
    if (g_show_hitboxes) return true;
    return orig_shouldDraw(self);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('H') & 1) {
            g_show_hitboxes = !g_show_hitboxes;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_show_hitboxes = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    cadence::hook_typed(
        "PlayLayer::shouldDrawDebugHitboxes()",
        hook_shouldDraw, &orig_shouldDraw);

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
