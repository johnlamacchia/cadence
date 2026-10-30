// FPS display — shows a live FPS counter in the top-left corner.
// Hooks CCDirector::drawScene to count frames and update the label.
#include <sdk.h>
#include <windows.h>
#include <cstdio>

static void* g_label = nullptr;
static int g_frame_count = 0;
static LARGE_INTEGER g_last_time;
static LARGE_INTEGER g_freq;

using sharedDirector_t = void* (*)();
using getRunningScene_t = void* (__fastcall*)(void*);
using getWinSize_t = void(__fastcall*)(void*, float* out_w, float* out_h);
using CCLabelBMFont_create_t = void* (*)(const char* str, const char* fnt);
using addChild_t = void(__fastcall*)(void*, void*, int);
using setPosition_t = void(__fastcall*)(void*, float, float);
using setScale_t = void(__fastcall*)(void*, float);
using setString_t = void(__fastcall*)(void*, const char*);
using setAnchorPoint_t = void(__fastcall*)(void*, float, float);
using setOpacity_t = void(__fastcall*)(void*, unsigned char);

static sharedDirector_t fn_sharedDirector = nullptr;
static getRunningScene_t fn_getRunningScene = nullptr;
static CCLabelBMFont_create_t fn_labelCreate = nullptr;
static addChild_t fn_addChild = nullptr;
static setPosition_t fn_setPosition = nullptr;
static setScale_t fn_setScale = nullptr;
static setString_t fn_setString = nullptr;
static setAnchorPoint_t fn_setAnchorPoint = nullptr;
static setOpacity_t fn_setOpacity = nullptr;
static getWinSize_t fn_getWinSize = nullptr;

static void(__fastcall* orig_drawScene)(void*) = nullptr;

static void ensure_label(void* director) {
    if (g_label) return;
    if (!fn_getRunningScene || !fn_labelCreate || !fn_addChild) return;

    void* scene = fn_getRunningScene(director);
    if (!scene) return;

    g_label = fn_labelCreate("FPS: --", "bigFont.fnt");
    if (!g_label) return;

    if (fn_setScale) fn_setScale(g_label, 0.4f);
    if (fn_setAnchorPoint) fn_setAnchorPoint(g_label, 0.0f, 1.0f);
    if (fn_setOpacity) fn_setOpacity(g_label, 180);

    // Position top-left. getWinSize returns CCSize by value (2 floats, fits in xmm0 on MSVC x64? No — returned via hidden pointer for structs > 8 bytes).
    // Simpler: just use hardcoded position, most GD is 1280x720 design resolution
    if (fn_setPosition) fn_setPosition(g_label, 5.0f, 715.0f);

    fn_addChild(scene, g_label, 99999);
}

static void __fastcall hook_drawScene(void* director) {
    orig_drawScene(director);

    g_frame_count++;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double elapsed = (double)(now.QuadPart - g_last_time.QuadPart) / (double)g_freq.QuadPart;

    if (elapsed >= 0.5) {
        double fps = g_frame_count / elapsed;
        g_frame_count = 0;
        g_last_time = now;

        ensure_label(director);

        if (g_label && fn_setString) {
            char buf[32];
            snprintf(buf, sizeof(buf), "FPS: %.0f", fps);
            fn_setString(g_label, buf);
        }
    }
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_label = nullptr;
    g_frame_count = 0;
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
    fn_setScale = (setScale_t)GetProcAddress(cocos,
        "?setScale@CCNode@cocos2d@@UEAAXM@Z");
    fn_setString = (setString_t)GetProcAddress(cocos,
        "?setString@CCLabelBMFont@cocos2d@@UEAAXPEBD@Z");
    fn_setAnchorPoint = (setAnchorPoint_t)GetProcAddress(cocos,
        "?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");
    fn_setOpacity = (setOpacity_t)GetProcAddress(cocos,
        "?setOpacity@CCNodeRGBA@cocos2d@@UEAAXE@Z");
    fn_getWinSize = (getWinSize_t)GetProcAddress(cocos,
        "?getWinSize@CCDirector@cocos2d@@QEAA?AVCCSize@2@XZ");

    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_last_time);

    // Hook drawScene from libcocos2d.dll
    void* drawScene = (void*)GetProcAddress(cocos,
        "?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene, (void**)&orig_drawScene);
    }
}
