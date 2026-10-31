// Attempt counter: counts level resets while a level is open and shows the total
// in the window title when you leave it.
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static int g_attempts = 0;

static bool __fastcall hook_init(PlayLayer* self, GJGameLevel* level, bool dontCreateObjects) {
    bool ok = cadence::original<&PlayLayer::init>(self, level, dontCreateObjects);
    if (ok) g_attempts = 0;
    return ok;
}

static void __fastcall hook_resetLevel(PlayLayer* self) {
    g_attempts++;
    cadence::original<&PlayLayer::resetLevel>(self);
}

static void __fastcall hook_onQuit(PlayLayer* self) {
    if (HWND hwnd = FindWindowA(nullptr, "Geometry Dash")) {
        char title[96];
        snprintf(title, sizeof(title), "Geometry Dash  |  Session: %d attempts", g_attempts);
        SetWindowTextA(hwnd, title);
    }
    cadence::original<&PlayLayer::onQuit>(self);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_attempts = 0;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::init>(hook_init);
    cadence::hook<&PlayLayer::resetLevel>(hook_resetLevel);
    cadence::hook<&PlayLayer::onQuit>(hook_onQuit);
}
