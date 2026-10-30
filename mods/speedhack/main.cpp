// Speed hack — press + to speed up, - to slow down, 0 to reset.
#include <sdk.h>
#include <windows.h>

static float g_speed = 1.0f;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

using getScheduler_t = void* (__fastcall*)(void*);
using setTimeScale_t = void(__fastcall*)(void*, float);
using sharedDirector_t = void* (*)();

static getScheduler_t g_getScheduler = nullptr;
static setTimeScale_t g_setTimeScale = nullptr;
static sharedDirector_t g_sharedDirector = nullptr;

static void apply_speed() {
    if (!g_sharedDirector || !g_getScheduler || !g_setTimeScale) return;
    void* director = g_sharedDirector();
    if (!director) return;
    void* scheduler = g_getScheduler(director);
    if (!scheduler) return;
    g_setTimeScale(scheduler, g_speed);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState(VK_OEM_PLUS) & 1) {
            g_speed += 0.25f;
            if (g_speed > 5.0f) g_speed = 5.0f;
            apply_speed();
        }
        if (GetAsyncKeyState(VK_OEM_MINUS) & 1) {
            g_speed -= 0.25f;
            if (g_speed < 0.25f) g_speed = 0.25f;
            apply_speed();
        }
        if (GetAsyncKeyState('0') & 1) {
            g_speed = 1.0f;
            apply_speed();
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_speed = 1.0f;
    apply_speed();
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return;

    g_sharedDirector = (sharedDirector_t)GetProcAddress(cocos,
        "?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    g_getScheduler = (getScheduler_t)GetProcAddress(cocos,
        "?getScheduler@CCDirector@cocos2d@@UEAAPEAVCCScheduler@2@XZ");
    g_setTimeScale = (setTimeScale_t)GetProcAddress(cocos,
        "?setTimeScale@CCScheduler@cocos2d@@QEAAXM@Z");

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
