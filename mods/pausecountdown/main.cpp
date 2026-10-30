// Pause countdown — after you press Resume, show 3-2-1 on screen before play continues.
// PauseLayer::onResume is the only caller of PlayLayer::resume, so we hold the
// resume back, count down on the render thread, then call the real resume.
#include <sdk.h>

using sharedDirector_t = void*(*)();
using getRunningScene_t = void*(__fastcall*)(void*);
using labelCreate_t = void*(*)(const char*, const char*);
using addChild_t = void(__fastcall*)(void*, void*, int);
using removeChild_t = void(__fastcall*)(void*, void*, bool);
using setPosition_t = void(__fastcall*)(void*, float, float);
using setScale_t = void(__fastcall*)(void*, float);

static sharedDirector_t fn_sharedDirector;
static getRunningScene_t fn_getRunningScene;
static labelCreate_t fn_labelCreate;
static addChild_t fn_addChild;
static removeChild_t fn_removeChild;
static setPosition_t fn_setPosition;
static setScale_t fn_setScale;

static void(__fastcall* orig_resume)(void* self);
static void(__fastcall* orig_drawScene)(void* director);

static void* g_pending = nullptr;  // the PlayLayer whose resume() we are delaying
static void* g_label = nullptr;
static void* g_label_scene = nullptr;
static int g_shown = 0;
static LARGE_INTEGER g_start, g_freq;

static void* running_scene() {
    void* dir = fn_sharedDirector ? fn_sharedDirector() : nullptr;
    return dir && fn_getRunningScene ? fn_getRunningScene(dir) : nullptr;
}

// Only touch the label if its scene is still the running one; otherwise the
// scene (and the label with it) was already released.
static void drop_label() {
    if (g_label && g_label_scene && running_scene() == g_label_scene)
        fn_removeChild(g_label_scene, g_label, true);
    g_label = nullptr;
    g_label_scene = nullptr;
}

static void show_number(int n) {
    drop_label();
    void* scene = running_scene();
    if (!scene) return;
    char text[8];
    snprintf(text, sizeof(text), "%d", n);
    void* label = fn_labelCreate(text, "bigFont.fnt");
    if (!label) return;
    fn_setScale(label, 1.6f);
    fn_setPosition(label, 284.5f, 160.0f);
    fn_addChild(scene, label, 100000);
    g_label = label;
    g_label_scene = scene;
}

static void __fastcall hook_resume(void* self) {
    if (g_pending) return;
    g_pending = self;
    g_shown = 0;
    QueryPerformanceCounter(&g_start);
}

static void __fastcall hook_drawScene(void* director) {
    if (g_pending) {
        void* pl = cadence::play_layer();
        if (pl != g_pending) {
            // The level was left mid-countdown; its scene is gone.
            g_pending = nullptr;
            g_label = nullptr;
            g_label_scene = nullptr;
        } else {
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            double elapsed = (double)(now.QuadPart - g_start.QuadPart) / (double)g_freq.QuadPart;
            int n = 3 - (int)elapsed;
            if (n <= 0) {
                drop_label();
                g_pending = nullptr;
                orig_resume(pl);
            } else if (n != g_shown) {
                g_shown = n;
                show_number(n);
            }
        }
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    // Never leave the game stuck paused if we are unloaded mid-countdown.
    if (g_pending && g_pending == cadence::play_layer()) {
        drop_label();
        orig_resume(g_pending);
    }
    g_pending = nullptr;
    g_label = nullptr;
    g_label_scene = nullptr;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Pause Countdown","desc":"3-2-1 countdown after unpausing","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    QueryPerformanceFrequency(&g_freq);

    fn_sharedDirector = (sharedDirector_t)cadence::cocos("?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)cadence::cocos("?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate = (labelCreate_t)cadence::cocos("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_addChild = (addChild_t)cadence::cocos("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_removeChild = (removeChild_t)cadence::cocos("?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");
    fn_setPosition = (setPosition_t)cadence::cocos("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_setScale = (setScale_t)cadence::cocos("?setScale@CCNode@cocos2d@@UEAAXM@Z");

    if (!fn_sharedDirector || !fn_getRunningScene || !fn_labelCreate || !fn_addChild ||
        !fn_removeChild || !fn_setPosition || !fn_setScale) {
        cadence::log("pausecountdown: missing cocos2d exports, not loading");
        return;
    }

    cadence::hook_typed("PlayLayer::resume()", hook_resume, &orig_resume);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
