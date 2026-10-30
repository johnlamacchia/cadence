// Speed hack — press + to speed up, - to slow down, 0 to reset.
// Hooks CCScheduler::update to multiply dt. The scheduler drives
// all game updates, so this one hook speeds up everything.
#include <sdk.h>
#include <windows.h>

static float g_speed = 1.0f;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;
static HWND g_hwnd = nullptr;

static void(__fastcall* orig_scheduler_update)(void*, float);

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt * g_speed);
}

static void update_title() {
    if (!g_hwnd || !IsWindow(g_hwnd)) return;
    if (g_speed == 1.0f) {
        SetWindowTextA(g_hwnd, "Geometry Dash");
    } else {
        char title[64];
        snprintf(title, sizeof(title), "Geometry Dash [%.2fx]", g_speed);
        SetWindowTextA(g_hwnd, title);
    }
}

static DWORD WINAPI key_thread(LPVOID) {
    bool was_plus = false, was_minus = false, was_reset = false;
    while (g_running) {
        bool changed = false;
        bool is_plus = ((GetAsyncKeyState(VK_OEM_PLUS) & 0x8000) || (GetAsyncKeyState(VK_ADD) & 0x8000)) != 0;
        bool is_minus = ((GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) || (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)) != 0;
        bool is_reset = (GetAsyncKeyState('0') & 0x8000) != 0;
        if (is_plus && !was_plus) {
            g_speed += 0.25f;
            if (g_speed > 5.0f) g_speed = 5.0f;
            changed = true;
        }
        if (is_minus && !was_minus) {
            g_speed -= 0.25f;
            if (g_speed < 0.25f) g_speed = 0.25f;
            changed = true;
        }
        if (is_reset && !was_reset) {
            g_speed = 1.0f;
            changed = true;
        }
        was_plus = is_plus;
        was_minus = is_minus;
        was_reset = is_reset;
        if (changed) update_title();
        Sleep(30);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_speed = 1.0f;
    if (g_hwnd && IsWindow(g_hwnd)) SetWindowTextA(g_hwnd, "Geometry Dash");
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"j({"name":"Speed Hack","desc":"Change game speed 0.25x-5x","key":"+/- 0","ver":"1.0"})j";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    g_hwnd = FindWindowA(nullptr, "Geometry Dash");

    void* schedUpdate = cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update,
            (void**)&orig_scheduler_update);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
