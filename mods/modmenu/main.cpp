// Mod Menu — press Tab to toggle an in-game overlay showing all loaded mods.
// Reads mod metadata from the loader API (cadence::get_mods).
// The overlay is a cocos2d layer added to the current scene.
#include <sdk.h>
#include <windows.h>

struct CCPoint { float x, y; };
struct _ccColor3B { unsigned char r, g, b; };
struct _ccColor4B { unsigned char r, g, b, a; };

using sharedDirector_t  = void*(*)();
using getRunningScene_t = void*(__fastcall*)(void*);
using getWinSize_t      = void(__fastcall*)(void*, float*, float*);
using CCLayer_init_t    = bool(__fastcall*)(void*);
using CCLayer_create_t  = void*(*)();
using addChild_t        = void(__fastcall*)(void*, void*, int);
using removeChild_t     = void(__fastcall*)(void*, void*, bool);
using setPosition_t     = void(__fastcall*)(void*, float, float);
using setScale_t        = void(__fastcall*)(void*, float);
using setAnchorPoint_t  = void(__fastcall*)(void*, const CCPoint*);
using setOpacity_t      = void(__fastcall*)(void*, unsigned char);
using setColor_t        = void(__fastcall*)(void*, const _ccColor3B*);
using CCLabelBMFont_create_t = void*(*)(const char*, const char*);
using CCLayerColor_create_t  = void*(*)(const _ccColor4B*, float, float);

static sharedDirector_t  fn_sharedDirector  = nullptr;
static getRunningScene_t fn_getRunningScene  = nullptr;
static CCLabelBMFont_create_t fn_labelCreate = nullptr;
static CCLayerColor_create_t fn_layerColor   = nullptr;
static addChild_t        fn_addChild         = nullptr;
static removeChild_t     fn_removeChild      = nullptr;
static setPosition_t     fn_setPosition      = nullptr;
static setScale_t        fn_setScale         = nullptr;
static setAnchorPoint_t  fn_setAnchorPoint   = nullptr;
static setOpacity_t      fn_setOpacity       = nullptr;
static setColor_t        fn_setColor         = nullptr;

static void* g_menu_layer = nullptr;
static void* g_menu_scene = nullptr;
static bool g_visible = false;
static volatile bool g_toggle_pending = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

static void(__fastcall* orig_drawScene)(void*);

static void* create_label(const char* text, float x, float y, float scale, _ccColor3B color) {
    void* label = fn_labelCreate(text, "bigFont.fnt");
    if (!label) return nullptr;
    CCPoint anchor = {0.0f, 1.0f};
    if (fn_setAnchorPoint) fn_setAnchorPoint(label, &anchor);
    if (fn_setScale) fn_setScale(label, scale);
    if (fn_setPosition) fn_setPosition(label, x, y);
    if (fn_setColor) fn_setColor(label, &color);
    return label;
}

static void destroy_menu() {
    if (g_menu_layer && g_menu_scene && fn_removeChild) {
        fn_removeChild(g_menu_scene, g_menu_layer, true);
    }
    g_menu_layer = nullptr;
    g_menu_scene = nullptr;
}

static void build_menu(void* scene) {
    destroy_menu();

    int mod_count = 0;
    const ModEntry* mods = cadence::get_mods(&mod_count);
    if (!mods || mod_count == 0) return;

    float panel_w = 340.0f;
    float row_h = 22.0f;
    float header_h = 32.0f;
    float pad = 10.0f;
    float panel_h = header_h + (mod_count * row_h) + pad * 2;
    if (panel_h > 500.0f) panel_h = 500.0f;

    _ccColor4B bg = {0, 0, 0, 180};
    g_menu_layer = fn_layerColor(&bg, panel_w, panel_h);
    if (!g_menu_layer) return;

    if (fn_setPosition) fn_setPosition(g_menu_layer, 10.0f, 320.0f - panel_h - 10.0f);

    _ccColor3B white = {255, 255, 255};
    _ccColor3B green = {100, 255, 100};
    _ccColor3B gray  = {150, 150, 150};
    _ccColor3B cyan  = {100, 200, 255};

    // Title
    char title[64];
    snprintf(title, sizeof(title), "Cadence  [%d mods]", mod_count);
    void* title_label = create_label(title, pad, panel_h - 6.0f, 0.35f, cyan);
    if (title_label) fn_addChild(g_menu_layer, title_label, 1);

    // Version
    char ver_str[32];
    snprintf(ver_str, sizeof(ver_str), "GD %s", cadence::gd_version());
    void* ver_label = create_label(ver_str, panel_w - 110.0f, panel_h - 6.0f, 0.25f, gray);
    if (ver_label) fn_addChild(g_menu_layer, ver_label, 1);

    // Mod list
    float y = panel_h - header_h - pad;
    for (int i = 0; i < mod_count && y > 0; i++) {
        const ModEntry& m = mods[i];
        const char* name = m.name ? m.name : m.filename;

        // Name
        void* name_label = create_label(name, pad, y, 0.28f, white);
        if (name_label) fn_addChild(g_menu_layer, name_label, 1);

        // Keybind (right side)
        if (m.keybind && m.keybind[0]) {
            char key_str[32];
            snprintf(key_str, sizeof(key_str), "[%s]", m.keybind);
            void* key_label = create_label(key_str, panel_w - 70.0f, y, 0.22f, green);
            if (key_label) fn_addChild(g_menu_layer, key_label, 1);
        }

        y -= row_h;
    }

    // "Tab to close" footer
    void* footer = create_label("Tab to close", pad, 12.0f, 0.2f, gray);
    if (footer) fn_addChild(g_menu_layer, footer, 1);

    fn_addChild(scene, g_menu_layer, 100000);
    g_menu_scene = scene;
}

static void __fastcall hook_drawScene(void* director) {
    if (g_toggle_pending) {
        g_toggle_pending = false;
        g_visible = !g_visible;

        if (g_visible) {
            void* scene = fn_getRunningScene(director);
            if (scene) build_menu(scene);
        } else {
            destroy_menu();
        }
    }

    // If scene changed while menu is open, rebuild on new scene
    if (g_visible && g_menu_layer) {
        void* scene = fn_getRunningScene(director);
        if (scene && scene != g_menu_scene) {
            g_menu_layer = nullptr;
            g_menu_scene = nullptr;
            build_menu(scene);
        }
    }

    orig_drawScene(director);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState(VK_TAB) & 1) {
            g_toggle_pending = true;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Mod Menu","desc":"In-game mod list overlay","key":"Tab","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    destroy_menu();
    g_visible = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    fn_sharedDirector  = (sharedDirector_t)cadence::cocos("?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)cadence::cocos("?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate     = (CCLabelBMFont_create_t)cadence::cocos("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_layerColor      = (CCLayerColor_create_t)cadence::cocos("?create@CCLayerColor@cocos2d@@SAPEAV12@AEBU_ccColor4B@2@MM@Z");
    fn_addChild        = (addChild_t)cadence::cocos("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_removeChild     = (removeChild_t)cadence::cocos("?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");
    fn_setPosition     = (setPosition_t)cadence::cocos("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_setScale        = (setScale_t)cadence::cocos("?setScale@CCNode@cocos2d@@UEAAXM@Z");
    fn_setAnchorPoint  = (setAnchorPoint_t)cadence::cocos("?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");
    fn_setOpacity      = (setOpacity_t)cadence::cocos("?setOpacity@CCNode@cocos2d@@UEAAXE@Z");
    fn_setColor        = (setColor_t)cadence::cocos("?setColor@CCNodeRGBA@cocos2d@@UEAAXAEBU_ccColor3B@2@@Z");

    if (!fn_labelCreate || !fn_layerColor || !fn_addChild || !fn_getRunningScene) {
        cadence::log("modmenu: missing cocos2d exports, aborting");
        return;
    }

    void* drawScene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene, (void**)&orig_drawScene);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
    cadence::log("modmenu: initialized, press Tab to open");
}
