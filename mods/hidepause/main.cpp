// Hide pause — press P in a level to hide the on-screen pause button.
#include <sdk.h>

static bool g_hide = false;
static void* g_ui = nullptr;  // most recent UILayer; only used while it belongs to the live PlayLayer

static void(__fastcall* fn_setVisible)(void* node, bool visible) = nullptr;
static bool(__fastcall* orig_UILayer_init)(void* self, void* gameLayer);
static void(__fastcall* orig_drawScene)(void* director);

static void* pause_button() {
    void* pl = cadence::play_layer();
    if (!pl || !g_ui) return nullptr;
    // The UILayer is only trusted if it was built for the PlayLayer that is alive now.
    if (*(void**)((char*)g_ui + cadence::off::ui_layer_game_layer) != pl) return nullptr;
    return *(void**)((char*)g_ui + cadence::off::ui_layer_pause_btn);
}

static bool __fastcall hook_UILayer_init(void* self, void* gameLayer) {
    bool ok = orig_UILayer_init(self, gameLayer);
    if (ok) {
        g_ui = self;
        if (g_hide && fn_setVisible) {
            void* btn = *(void**)((char*)self + cadence::off::ui_layer_pause_btn);
            if (btn) fn_setVisible(btn, false);
        }
    }
    return ok;
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::play_layer() && cadence::key_pressed('P')) {
        g_hide = !g_hide;
        void* btn = pause_button();
        if (btn && fn_setVisible) fn_setVisible(btn, !g_hide);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    void* btn = pause_button();
    if (g_hide && btn && fn_setVisible) fn_setVisible(btn, true);
    g_hide = false;
    g_ui = nullptr;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hide Pause","desc":"Hides the pause button","key":"P","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_setVisible = (decltype(fn_setVisible))cadence::cocos("?setVisible@CCNode@cocos2d@@UEAAX_N@Z");

    cadence::hook_typed("UILayer::init(GJBaseGameLayer* layer)", hook_UILayer_init, &orig_UILayer_init);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
