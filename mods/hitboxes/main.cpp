// Hitbox viewer — press H to toggle debug hitbox rendering.
#include <sdk.h>
#include <windows.h>

static bool g_show_hitboxes = false;

static bool(__fastcall* orig_shouldDraw)(void* self);

static bool __fastcall hook_shouldDraw(void* self) {
    if (g_show_hitboxes) return true;
    return orig_shouldDraw(self);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (true) {
        if (GetAsyncKeyState('H') & 1) {
            g_show_hitboxes = !g_show_hitboxes;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_show_hitboxes = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::shouldDrawDebugHitboxes()",
        hook_shouldDraw, &orig_shouldDraw);

    CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
