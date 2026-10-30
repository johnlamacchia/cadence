// Mod Menu — press Insert to toggle an in-game overlay showing all loaded mods.
#include <sdk.h>
#include <windows.h>

struct CCPoint { float x, y; };
struct _ccColor4B { unsigned char r, g, b, a; };

using sharedDirector_t      = void*(*)();
using getRunningScene_t     = void*(__fastcall*)(void*);
using addChild_t            = void(__fastcall*)(void*, void*, int);
using removeChild_t         = void(__fastcall*)(void*, void*, bool);
using setPosition_t         = void(__fastcall*)(void*, float, float);
using setScale_t            = void(__fastcall*)(void*, float);
using setAnchorPoint_t      = void(__fastcall*)(void*, const CCPoint*);
using CCLabelBMFont_create_t = void*(*)(const char*, const char*);
using CCLayerColor_create_t  = void*(*)(const _ccColor4B*, float, float);

static sharedDirector_t      fn_sharedDirector  = nullptr;
static getRunningScene_t     fn_getRunningScene  = nullptr;
static CCLabelBMFont_create_t fn_labelCreate     = nullptr;
static CCLayerColor_create_t fn_layerColor       = nullptr;
static addChild_t            fn_addChild         = nullptr;
static removeChild_t         fn_removeChild      = nullptr;
static setPosition_t         fn_setPosition      = nullptr;
static setScale_t            fn_setScale         = nullptr;
static setAnchorPoint_t      fn_setAnchorPoint   = nullptr;
static void* g_panel = nullptr;
static void* g_menu_scene = nullptr;
static bool g_visible = false;

static void(__fastcall* orig_drawScene)(void*);

static void* make_label(const char* text, float x, float y, float scale, const char* font = "bigFont.fnt") {
    if (!text || !text[0]) return nullptr;
    void* label = fn_labelCreate(text, font);
    if (!label) return nullptr;
    CCPoint anchor = {0.0f, 1.0f};
    if (fn_setAnchorPoint) fn_setAnchorPoint(label, &anchor);
    if (fn_setScale) fn_setScale(label, scale);
    if (fn_setPosition) fn_setPosition(label, x, y);
    return label;
}

static void destroy_menu() {
    if (g_panel && g_menu_scene && fn_removeChild) {
        fn_removeChild(g_menu_scene, g_panel, true);
    }
    g_panel = nullptr;
    g_menu_scene = nullptr;
}

static void build_menu(void* scene) {
    destroy_menu();

    int mod_count = 0;
    const ModEntry* mods = cadence::get_mods(&mod_count);
    if (!mods || mod_count == 0) return;

    float screen_h = 320.0f;
    float margin = 8.0f;
    float max_panel_h = screen_h - margin * 2;
    float pad = 10.0f;
    float header_h = 30.0f;
    float footer_h = 20.0f;
    float panel_w = 340.0f;

    // Calculate row height to fit all mods on screen
    float avail = max_panel_h - header_h - footer_h - pad;
    float row_h = avail / (float)mod_count;
    if (row_h > 20.0f) row_h = 20.0f;
    if (row_h < 12.0f) row_h = 12.0f;

    float panel_h = header_h + (mod_count * row_h) + footer_h + pad;
    if (panel_h > max_panel_h) panel_h = max_panel_h;

    float label_scale = (row_h >= 18.0f) ? 0.28f : (row_h >= 14.0f) ? 0.22f : 0.18f;
    float key_scale = label_scale * 0.8f;

    // Dark background panel
    if (fn_layerColor) {
        _ccColor4B bg = {0, 0, 0, 190};
        g_panel = fn_layerColor(&bg, panel_w, panel_h);
    }
    if (!g_panel) return;

    if (fn_setPosition) fn_setPosition(g_panel, margin, screen_h - panel_h - margin);

    // Title — gold font
    char title[64];
    snprintf(title, sizeof(title), "Cadence  [%d mods]", mod_count);
    void* lbl = make_label(title, pad, panel_h - 6.0f, 0.35f, "goldFont.fnt");
    if (lbl) fn_addChild(g_panel, lbl, 1);

    // GD version — small, right side
    char ver[32];
    snprintf(ver, sizeof(ver), "GD %s", cadence::gd_version());
    lbl = make_label(ver, panel_w - 120.0f, panel_h - 10.0f, 0.22f, "chatFont.fnt");
    if (lbl) fn_addChild(g_panel, lbl, 1);

    // Mod list
    float y = panel_h - header_h - 6.0f;
    for (int i = 0; i < mod_count && y > footer_h; i++) {
        const char* name = nullptr;
        if (mods[i].name && (uintptr_t)mods[i].name > 0x10000)
            name = mods[i].name;
        else if (mods[i].filename && (uintptr_t)mods[i].filename > 0x10000)
            name = mods[i].filename;
        else
            name = "???";

        lbl = make_label(name, pad, y, label_scale);
        if (lbl) fn_addChild(g_panel, lbl, 1);

        if (mods[i].keybind && (uintptr_t)mods[i].keybind > 0x10000 && mods[i].keybind[0]) {
            char key[32];
            snprintf(key, sizeof(key), "[%s]", mods[i].keybind);
            lbl = make_label(key, panel_w - 80.0f, y, key_scale, "goldFont.fnt");
            if (lbl) fn_addChild(g_panel, lbl, 1);
        }

        y -= row_h;
    }

    // Footer
    lbl = make_label("Insert to close", pad, 8.0f, 0.2f, "chatFont.fnt");
    if (lbl) fn_addChild(g_panel, lbl, 1);

    fn_addChild(scene, g_panel, 100000);
    g_menu_scene = scene;
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::key_pressed(VK_INSERT)) {
        g_visible = !g_visible;

        if (g_visible) {
            void* scene = fn_getRunningScene(director);
            if (scene) build_menu(scene);
        } else {
            destroy_menu();
        }
    }

    if (g_visible) {
        void* scene = fn_getRunningScene(director);
        if (scene && (!g_panel || scene != g_menu_scene)) {
            g_panel = nullptr;
            g_menu_scene = nullptr;
            build_menu(scene);
        }
    }

    orig_drawScene(director);
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Mod Menu","desc":"In-game mod list overlay","key":"Insert","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_unload() {
    destroy_menu();
    g_visible = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_sharedDirector  = (sharedDirector_t)cadence::cocos("?sharedDirector@CCDirector@cocos2d@@SAPEAV12@XZ");
    fn_getRunningScene = (getRunningScene_t)cadence::cocos("?getRunningScene@CCDirector@cocos2d@@QEAAPEAVCCScene@2@XZ");
    fn_labelCreate     = (CCLabelBMFont_create_t)cadence::cocos("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
    fn_layerColor      = (CCLayerColor_create_t)cadence::cocos("?create@CCLayerColor@cocos2d@@SAPEAV12@AEBU_ccColor4B@2@MM@Z");
    fn_addChild        = (addChild_t)cadence::cocos("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
    fn_removeChild     = (removeChild_t)cadence::cocos("?removeChild@CCNode@cocos2d@@UEAAXPEAV12@_N@Z");
    fn_setPosition     = (setPosition_t)cadence::cocos("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
    fn_setScale        = (setScale_t)cadence::cocos("?setScale@CCNode@cocos2d@@UEAAXM@Z");
    fn_setAnchorPoint  = (setAnchorPoint_t)cadence::cocos("?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");

    if (!fn_labelCreate || !fn_addChild || !fn_getRunningScene) {
        cadence::log("modmenu: missing cocos2d exports, aborting");
        return;
    }

    void* drawScene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene, (void**)&orig_drawScene);
    }

    cadence::log("modmenu: initialized, press Insert to open");
}
