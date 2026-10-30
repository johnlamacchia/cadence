// FPS display — on-screen label showing live FPS in the top-left corner.
// Uses two labels: a static "FPS:" prefix and a dynamic number that updates.
#include <sdk.h>
#include <windows.h>

struct CCPoint { float x, y; };

static void* g_label_prefix = nullptr;
static void* g_label_value = nullptr;
static void* g_label_scene = nullptr;
static int g_scene_gen = 0;
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
using setScale_t = void(__fastcall*)(void*, float);
using setAnchorPoint_t = void(__fastcall*)(void*, const CCPoint*);

static sharedDirector_t fn_sharedDirector = nullptr;
static getRunningScene_t fn_getRunningScene = nullptr;
static CCLabelBMFont_create_t fn_labelCreate = nullptr;
static addChild_t fn_addChild = nullptr;
static setPosition_t fn_setPosition = nullptr;
static removeChild_t fn_removeChild = nullptr;
static setScale_t fn_setScale = nullptr;
static setAnchorPoint_t fn_setAnchorPoint = nullptr;

static void(__fastcall* orig_scheduler_update)(void*, float) = nullptr;
static bool(__fastcall* orig_MenuLayer_init)(void* self) = nullptr;

static bool __fastcall hook_MenuLayer_init(void* self) {
    bool ret = orig_MenuLayer_init(self);
    g_game_ready = true;
    g_label_prefix = nullptr;
    g_label_value = nullptr;
    g_label_scene = nullptr;
    g_scene_gen++;
    return ret;
}

static void* create_label(const char* text, float x, float y) {
    void* label = fn_labelCreate(text, "bigFont.fnt");
    if (!label) return nullptr;
    CCPoint anchor = {0.0f, 1.0f};
    if (fn_setAnchorPoint) fn_setAnchorPoint(label, &anchor);
    if (fn_setScale) fn_setScale(label, 0.35f);
    if (fn_setPosition) fn_setPosition(label, x, y);
    return label;
}

static void ensure_prefix(void* scene) {
    if (g_label_prefix) return;
    g_label_prefix = create_label("FPS:", 5.0f, 310.0f);
    if (g_label_prefix) fn_addChild(scene, g_label_prefix, 99999);
}

static void update_value(void* scene, const char* text) {
    if (g_label_value && fn_removeChild) {
        fn_removeChild(scene, g_label_value, true);
        g_label_value = nullptr;
    }
    g_label_value = create_label(text, 55.0f, 310.0f);
    if (g_label_value) fn_addChild(scene, g_label_value, 99999);
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (!g_game_ready) return;
    if (g_warmup_frames < 120) { g_warmup_frames++; return; }
    if (!fn_sharedDirector || !fn_getRunningScene || !fn_labelCreate || !fn_addChild) return;

    void* director = fn_sharedDirector();
    if (!director) return;
    void* scene = fn_getRunningScene(director);
    if (!scene) return;

    bool scene_changed = (scene != g_label_scene);
    if (scene_changed) {
        g_label_prefix = nullptr;
        g_label_value = nullptr;
        g_label_scene = scene;
        g_scene_gen++;
        g_frame_count = 0;
        QueryPerformanceCounter(&g_last_time);
    }

    ensure_prefix(scene);

    g_frame_count++;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = (double)(now.QuadPart - g_last_time.QuadPart) / (double)g_freq.QuadPart;

    if (elapsed >= 0.25) {
        double fps = g_frame_count / elapsed;
        g_frame_count = 0;
        g_last_time = now;

        char buf[16];
        snprintf(buf, sizeof(buf), "%.0f", fps);
        update_value(scene, buf);
    }
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_label_prefix = nullptr;
    g_label_value = nullptr;
    g_label_scene = nullptr;
    g_game_ready = false;
    g_frame_count = 0;
    g_warmup_frames = 0;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"j({"name":"FPS Display","desc":"On-screen FPS counter","ver":"1.0"})j";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_sharedDirector = (sharedDirector_t)cadence::cocos("?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)cadence::cocos("?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate = (CCLabelBMFont_create_t)cadence::cocos("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_addChild = (addChild_t)cadence::cocos("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_setPosition = (setPosition_t)cadence::cocos("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_removeChild = (removeChild_t)cadence::cocos("?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");
    fn_setScale = (setScale_t)cadence::cocos("?setScale@CCNode@cocos2d@@UEAAXM@Z");
    fn_setAnchorPoint = (setAnchorPoint_t)cadence::cocos("?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");

    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_last_time);

    cadence::hook_typed("MenuLayer::init()", hook_MenuLayer_init, &orig_MenuLayer_init);

    void* schedUpdate = cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update, (void**)&orig_scheduler_update);
    }
}
