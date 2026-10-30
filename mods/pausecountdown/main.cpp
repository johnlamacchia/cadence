// Pause countdown — shows a 3-2-1 countdown in the window title after unpausing.
// Uses deferred timing via scheduler hook so the game doesn't freeze.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static HWND g_hwnd = nullptr;
static volatile int g_countdown = 0;
static LARGE_INTEGER g_countdown_start;
static LARGE_INTEGER g_freq;
static void* g_stashed_self = nullptr;

static void(__fastcall* orig_resume)(void* self);
static void(__fastcall* orig_scheduler_update)(void*, float);

static HWND find_gd_window() {
    return FindWindowA(nullptr, "Geometry Dash");
}

static void __fastcall hook_resume(void* self) {
    if (!g_hwnd) g_hwnd = find_gd_window();
    g_stashed_self = self;
    g_countdown = 3;
    QueryPerformanceCounter(&g_countdown_start);
    if (g_hwnd) SetWindowTextA(g_hwnd, "Geometry Dash  |  Resuming in 3...");
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (g_countdown > 0) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        double elapsed = (double)(now.QuadPart - g_countdown_start.QuadPart) / (double)g_freq.QuadPart;

        int remaining = 3 - (int)elapsed;
        if (remaining < 0) remaining = 0;

        if (remaining != g_countdown) {
            g_countdown = remaining;
            if (g_hwnd) {
                if (remaining > 0) {
                    char title[64];
                    snprintf(title, sizeof(title), "Geometry Dash  |  Resuming in %d...", remaining);
                    SetWindowTextA(g_hwnd, title);
                } else {
                    SetWindowTextA(g_hwnd, "Geometry Dash");
                    if (g_stashed_self && orig_resume) {
                        orig_resume(g_stashed_self);
                    }
                    g_stashed_self = nullptr;
                }
            }
        }
    }
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_countdown = 0;
    g_stashed_self = nullptr;
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

    void* schedUpdate = cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update,
            (void**)&orig_scheduler_update);
    }
}
