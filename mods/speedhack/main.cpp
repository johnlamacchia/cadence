// Speed hack — in a level: + faster, - slower, 0 resets (0.25x - 5x).
// Scales the dt that CCScheduler::update hands to every scheduled game update.
#include <sdk.h>

static float g_speed = 1.0f;
static HWND g_hwnd = nullptr;

static void(__fastcall* orig_scheduler_update)(void* scheduler, float dt);
static void(__fastcall* orig_drawScene)(void* director);

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

static void __fastcall hook_drawScene(void* director) {
    bool plus  = cadence::key_pressed(VK_OEM_PLUS)  | cadence::key_pressed(VK_ADD);
    bool minus = cadence::key_pressed(VK_OEM_MINUS) | cadence::key_pressed(VK_SUBTRACT);
    bool reset = cadence::key_pressed('0');

    if (cadence::play_layer() && (plus || minus || reset)) {
        if (plus)  g_speed = g_speed + 0.25f > 5.0f  ? 5.0f  : g_speed + 0.25f;
        if (minus) g_speed = g_speed - 0.25f < 0.25f ? 0.25f : g_speed - 0.25f;
        if (reset) g_speed = 1.0f;
        update_title();
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_speed = 1.0f;
    update_title();
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"j({"name":"Speed Hack","desc":"Change game speed 0.25x-5x","key":"+/- 0","ver":"1.1"})j";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    g_hwnd = FindWindowA(nullptr, "Geometry Dash");

    cadence::hook(cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z"),
                  (void*)hook_scheduler_update, (void**)&orig_scheduler_update);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
