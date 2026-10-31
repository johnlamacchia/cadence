// FPS display: a live FPS counter in the top-left corner of every scene.
// Two labels: a fixed "FPS:" and a number that is replaced a few times a second.
#include <sdk.h>
#include <gd/MenuLayer.hpp>

using sharedDirector_t = void* (*)();
using getRunningScene_t = void* (__fastcall*)(void*);
using labelCreate_t = void* (*)(const char* text, const char* font);
using addChild_t = void(__fastcall*)(void* parent, void* child, int z);
using removeChild_t = void(__fastcall*)(void* parent, void* child, bool cleanup);
using setPosition_t = void(__fastcall*)(void* node, float x, float y);
using setScale_t = void(__fastcall*)(void* node, float s);
using setAnchorPoint_t = void(__fastcall*)(void* node, const cocos2d::CCPoint* p);

static sharedDirector_t fn_sharedDirector;
static getRunningScene_t fn_getRunningScene;
static labelCreate_t fn_labelCreate;
static addChild_t fn_addChild;
static removeChild_t fn_removeChild;
static setPosition_t fn_setPosition;
static setScale_t fn_setScale;
static setAnchorPoint_t fn_setAnchorPoint;

static void(__fastcall* orig_scheduler_update)(void* scheduler, float dt);

static void* g_prefix = nullptr;
static void* g_value = nullptr;
static void* g_scene = nullptr;
static bool g_menu_seen = false;
static int g_warmup_frames = 0;
static int g_frames = 0;
static LARGE_INTEGER g_last, g_freq;

static bool __fastcall hook_MenuLayer_init(MenuLayer* self) {
    bool ok = cadence::original<&MenuLayer::init>(self);
    g_menu_seen = true;                 // the game is fully up once the main menu exists
    g_prefix = g_value = g_scene = nullptr;
    return ok;
}

static void* make_label(const char* text, float x, float y) {
    void* label = fn_labelCreate(text, "bigFont.fnt");
    if (!label) return nullptr;
    cocos2d::CCPoint anchor = {0.0f, 1.0f};
    fn_setAnchorPoint(label, &anchor);
    fn_setScale(label, 0.35f);
    fn_setPosition(label, x, y);
    return label;
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (!g_menu_seen) return;
    if (g_warmup_frames < 120) { g_warmup_frames++; return; }

    void* director = fn_sharedDirector();
    void* scene = director ? fn_getRunningScene(director) : nullptr;
    if (!scene) return;

    if (scene != g_scene) {             // new scene: the old labels went with it
        g_scene = scene;
        g_prefix = g_value = nullptr;
        g_frames = 0;
        QueryPerformanceCounter(&g_last);
    }
    if (!g_prefix) {
        g_prefix = make_label("FPS:", 5.0f, 310.0f);
        if (g_prefix) fn_addChild(scene, g_prefix, 99999);
    }

    g_frames++;
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = double(now.QuadPart - g_last.QuadPart) / double(g_freq.QuadPart);
    if (elapsed < 0.25) return;

    char text[16];
    snprintf(text, sizeof(text), "%.0f", g_frames / elapsed);
    g_frames = 0;
    g_last = now;

    if (g_value) fn_removeChild(scene, g_value, true);
    g_value = make_label(text, 55.0f, 310.0f);
    if (g_value) fn_addChild(scene, g_value, 99999);
}

extern "C" __declspec(dllexport) void mod_unload() {
    // Take our labels out of the scene we put them in (if it is still the running one).
    void* director = fn_sharedDirector ? fn_sharedDirector() : nullptr;
    void* scene = director ? fn_getRunningScene(director) : nullptr;
    if (scene && scene == g_scene) {
        if (g_prefix) fn_removeChild(scene, g_prefix, true);
        if (g_value) fn_removeChild(scene, g_value, true);
    }
    g_prefix = g_value = g_scene = nullptr;
    g_menu_seen = false;
    g_warmup_frames = 0;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_sharedDirector = (sharedDirector_t)cadence::cocos("?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)cadence::cocos("?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate = (labelCreate_t)cadence::cocos("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_addChild = (addChild_t)cadence::cocos("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_removeChild = (removeChild_t)cadence::cocos("?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");
    fn_setPosition = (setPosition_t)cadence::cocos("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_setScale = (setScale_t)cadence::cocos("?setScale@CCNode@cocos2d@@UEAAXM@Z");
    fn_setAnchorPoint = (setAnchorPoint_t)cadence::cocos("?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");

    if (!fn_sharedDirector || !fn_getRunningScene || !fn_labelCreate || !fn_addChild || !fn_removeChild ||
        !fn_setPosition || !fn_setScale || !fn_setAnchorPoint) {
        cadence::log("fpsdisplay: missing cocos2d exports, not loading");
        return;
    }

    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_last);

    cadence::hook<&MenuLayer::init>(hook_MenuLayer_init);
    cadence::hook(cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z"),
                  (void*)hook_scheduler_update, (void**)&orig_scheduler_update);
}
