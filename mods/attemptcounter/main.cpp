// Attempt counter — tracks and displays total attempts in window title while in a level.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static int g_attempts = 0;
static bool g_in_level = false;
static HWND g_hwnd = nullptr;

static HWND find_gd_window() {
    return FindWindowA(nullptr, "Geometry Dash");
}

static void update_title() {
    if (!g_hwnd) g_hwnd = find_gd_window();
    // Don't set title while in level — fpsdisplay handles title updates.
    // Instead just track attempts; the counter shows on level complete/quit.
}

static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    bool ret = orig_PlayLayer_init(self, level, dontCreate);
    if (ret) {
        g_in_level = true;
        g_attempts = 0;
    }
    return ret;
}

static void(__fastcall* orig_resetLevel)(void* self);

static void __fastcall hook_resetLevel(void* self) {
    g_attempts++;
    orig_resetLevel(self);
}

static void(__fastcall* orig_PlayLayer_onQuit)(void* self);

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_in_level = false;
    if (!g_hwnd) g_hwnd = find_gd_window();
    if (g_hwnd) {
        char title[128];
        snprintf(title, sizeof(title), "Geometry Dash  |  Session: %d attempts", g_attempts);
        SetWindowTextA(g_hwnd, title);
    }
    orig_PlayLayer_onQuit(self);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_in_level = false;
    g_attempts = 0;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::resetLevel()",
        hook_resetLevel, &orig_resetLevel);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);
}
