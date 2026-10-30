// Pause countdown — shows a 3-2-1 countdown in the window title after unpausing.
// Uses drawScene hook (runs every frame even while paused) for the timer.
// Defers orig_resume until countdown completes.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static HWND g_hwnd = nullptr;
static void* g_stashed_self = nullptr;
static volatile bool g_countdown_active = false;
static int g_countdown_display = 0;
static LARGE_INTEGER g_countdown_start;
static LARGE_INTEGER g_freq;
static volatile bool g_in_level = false;

static void(__fastcall* orig_resume)(void* self);
static void(__fastcall* orig_drawScene)(void*);
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dc);

static HWND find_gd_window() {
    return FindWindowA(nullptr, "Geometry Dash");
}

static void __fastcall hook_resume(void* self) {
    if (!g_hwnd) g_hwnd = find_gd_window();
    g_stashed_self = self;
    g_countdown_display = 3;
    g_countdown_active = true;
    QueryPerformanceCounter(&g_countdown_start);
    if (g_hwnd) SetWindowTextA(g_hwnd, "Geometry Dash  |  Resuming in 3...");
}

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dc) {
    bool ret = orig_PlayLayer_init(self, level, dc);
    g_in_level = true;
    return ret;
}

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    if (g_countdown_active && g_stashed_self) {
        g_countdown_active = false;
        orig_resume(g_stashed_self);
        g_stashed_self = nullptr;
        if (g_hwnd) SetWindowTextA(g_hwnd, "Geometry Dash");
    }
    g_in_level = false;
    orig_PlayLayer_onQuit(self);
}

static void __fastcall hook_drawScene(void* director) {
    if (g_countdown_active && g_stashed_self) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        double elapsed = (double)(now.QuadPart - g_countdown_start.QuadPart) / (double)g_freq.QuadPart;

        int remaining = 3 - (int)elapsed;
        if (remaining < 0) remaining = 0;

        if (remaining != g_countdown_display) {
            g_countdown_display = remaining;
            if (remaining > 0) {
                char title[64];
                snprintf(title, sizeof(title), "Geometry Dash  |  Resuming in %d...", remaining);
                if (g_hwnd) SetWindowTextA(g_hwnd, title);
            } else {
                if (g_hwnd) SetWindowTextA(g_hwnd, "Geometry Dash");
                g_countdown_active = false;
                orig_resume(g_stashed_self);
                g_stashed_self = nullptr;
            }
        }
    }

    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_countdown_active && g_stashed_self && orig_resume) {
        orig_resume(g_stashed_self);
    }
    g_countdown_active = false;
    g_stashed_self = nullptr;
    g_in_level = false;
    if (g_hwnd && IsWindow(g_hwnd)) {
        SetWindowTextA(g_hwnd, "Geometry Dash");
        g_hwnd = nullptr;
    }
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Pause Countdown","desc":"3-2-1 countdown after unpausing","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    QueryPerformanceFrequency(&g_freq);

    cadence::hook_typed(
        "PlayLayer::resume()",
        hook_resume, &orig_resume);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    void* drawScene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene,
            (void**)&orig_drawScene);
    }
}
