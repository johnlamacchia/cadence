// FPS display — on-screen label showing live FPS in the top-left corner.
// Recreates a CCLabelBMFont each update instead of calling setString,
// because the setString export is a this-adjuster thunk that corrupts
// the pointer when called directly (not through vtable).
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static void* g_label = nullptr;
static void* g_label_scene = nullptr;
static bool g_game_ready = false;
static int g_frame_count = 0;
static int g_warmup_frames = 0;
static LARGE_INTEGER g_last_time;
static LARGE_INTEGER g_freq;

using sharedDirector_t = void* (*)();
using getRunningScene_t = void* (__fastcall*)(void*);
using CCLabelBMFont_create_t = void* (*)(const char* str, const char* fnt);
using addChild_t = void(__fastcall*)(void*, void*, int);
using setPosition_t = void(__fastcall*)(void*, float, float);
using removeChild_t = void(__fastcall*)(void*, void*, bool);

static sharedDirector_t fn_sharedDirector = nullptr;
static getRunningScene_t fn_getRunningScene = nullptr;
static CCLabelBMFont_create_t fn_labelCreate = nullptr;
static addChild_t fn_addChild = nullptr;
static setPosition_t fn_setPosition = nullptr;
static removeChild_t fn_removeChild = nullptr;

static void(__fastcall* orig_scheduler_update)(void*, float) = nullptr;
static bool(__fastcall* orig_MenuLayer_init)(void* self) = nullptr;

static bool __fastcall hook_MenuLayer_init(void* self) {
    bool ret = orig_MenuLayer_init(self);
    g_game_ready = true;
    return ret;
}

static void update_label(const char* text) {
    if (!fn_sharedDirector || !fn_getRunningScene || !fn_labelCreate || !fn_addChild) return;

    void* director = fn_sharedDirector();
    if (!director) return;
    void* scene = fn_getRunningScene(director);
    if (!scene) return;

    if (scene != g_label_scene) {
        g_label = nullptr;
        g_label_scene = nullptr;
    }

    // Remove old label
    if (g_label && fn_removeChild) {
        fn_removeChild(g_label_scene, g_label, true);
        g_label = nullptr;
    }

    // Create new label with updated text
    g_label = fn_labelCreate(text, "bigFont.fnt");
    if (!g_label) return;

    if (fn_setPosition) fn_setPosition(g_label, 60.0f, 290.0f);
    fn_addChild(scene, g_label, 99999);
    g_label_scene = scene;
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (!g_game_ready) return;

    if (g_warmup_frames < 120) {
        g_warmup_frames++;
        return;
    }

    g_frame_count++;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = (double)(now.QuadPart - g_last_time.QuadPart) / (double)g_freq.QuadPart;

    if (elapsed >= 1.0) {
        double fps = g_frame_count / elapsed;
        g_frame_count = 0;
        g_last_time = now;

        char buf[32];
        snprintf(buf, sizeof(buf), "FPS: %.0f", fps);
        update_label(buf);
    }
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_label = nullptr;
    g_label_scene = nullptr;
    g_game_ready = false;
    g_frame_count = 0;
    g_warmup_frames = 0;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (!cocos) return;

    fn_sharedDirector = (sharedDirector_t)GetProcAddress(cocos,
        "?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)GetProcAddress(cocos,
        "?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate = (CCLabelBMFont_create_t)GetProcAddress(cocos,
        "?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_addChild = (addChild_t)GetProcAddress(cocos,
        "?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_setPosition = (setPosition_t)GetProcAddress(cocos,
        "?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_removeChild = (removeChild_t)GetProcAddress(cocos,
        "?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");

    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_last_time);

    cadence::hook_typed("MenuLayer::init()", hook_MenuLayer_init, &orig_MenuLayer_init);

    void* schedUpdate = (void*)GetProcAddress(cocos,
        "?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update, (void**)&orig_scheduler_update);
    }
}
