// FPS display — shows live FPS in the window title bar.
// Zero cocos2d dependencies, just Win32 API.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static HWND g_hwnd = nullptr;

static HWND find_gd_window() {
    return FindWindowA(nullptr, "Geometry Dash");
}

static void(__fastcall* orig_scheduler_update)(void*, float) = nullptr;
static int g_frame_count = 0;
static LARGE_INTEGER g_freq;
static LARGE_INTEGER g_last_time;

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    g_frame_count++;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = (double)(now.QuadPart - g_last_time.QuadPart) / (double)g_freq.QuadPart;

    if (elapsed >= 1.0) {
        double fps = g_frame_count / elapsed;
        g_frame_count = 0;
        g_last_time = now;

        if (!g_hwnd) g_hwnd = find_gd_window();
        if (g_hwnd) {
            char title[128];
            snprintf(title, sizeof(title), "Geometry Dash  |  FPS: %.0f", fps);
            SetWindowTextA(g_hwnd, title);
        }
    }
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_hwnd) {
        SetWindowTextA(g_hwnd, "Geometry Dash");
        g_hwnd = nullptr;
    }
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_last_time);

    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return;

    void* schedUpdate = (void*)GetProcAddress(cocos,
        "?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update, (void**)&orig_scheduler_update);
    }
}
