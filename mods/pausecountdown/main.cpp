// Pause countdown — shows a 3-2-1 countdown in the window title after unpausing.
// Hooks PlayLayer::resume to inject a delay before gameplay resumes.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static HWND g_hwnd = nullptr;

static HWND find_gd_window() {
    return FindWindowA(nullptr, "Geometry Dash");
}

static void(__fastcall* orig_resume)(void* self);

static void __fastcall hook_resume(void* self) {
    if (!g_hwnd) g_hwnd = find_gd_window();
    if (!g_hwnd) {
        orig_resume(self);
        return;
    }

    for (int i = 3; i >= 1; i--) {
        char title[64];
        snprintf(title, sizeof(title), "Geometry Dash  |  Resuming in %d...", i);
        SetWindowTextA(g_hwnd, title);
        Sleep(400);
    }

    SetWindowTextA(g_hwnd, "Geometry Dash");
    orig_resume(self);
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_hwnd) {
        SetWindowTextA(g_hwnd, "Geometry Dash");
        g_hwnd = nullptr;
    }
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Pause Countdown","desc":"3-2-1 countdown after unpausing","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::resume()",
        hook_resume, &orig_resume);
}
