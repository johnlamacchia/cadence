// Hide pause: press P in a level to hide the on-screen pause button.
#include <sdk_extras.h>
#include <gd/PlayLayer.hpp>
#include <gd/UILayer.hpp>
#include <gd/CCMenuItemSpriteExtra.hpp>

static bool g_hide = false;
static UILayer* g_ui = nullptr;   // the UILayer built most recently; trusted only while it belongs to the live PlayLayer

static void(__fastcall* fn_setVisible)(void* node, bool visible) = nullptr;
static void(__fastcall* orig_drawScene)(void* director);

// The pause button of the level that is open right now, or nullptr.
static CCMenuItemSpriteExtra* pause_button() {
    PlayLayer* pl = cadence::play_layer();
    if (!pl || !g_ui) return nullptr;
    // A UILayer from an earlier level (or the editor) is not ours to touch.
    if (g_ui->m_gameLayer != pl) return nullptr;
    return g_ui->m_pauseBtn;
}

static bool __fastcall hook_UILayer_init(UILayer* self, GJBaseGameLayer* layer) {
    bool ok = cadence::original<&UILayer::init>(self, layer);
    if (ok) {
        g_ui = self;
        if (g_hide && self->m_pauseBtn) fn_setVisible(self->m_pauseBtn, false);
    }
    return ok;
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::play_layer() && cadence::key_pressed('P')) {
        g_hide = !g_hide;
        if (auto* btn = pause_button()) fn_setVisible(btn, !g_hide);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    if (g_hide) {
        if (auto* btn = pause_button()) fn_setVisible(btn, true);
    }
    g_hide = false;
    g_ui = nullptr;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_setVisible = (decltype(fn_setVisible))cadence::cocos("?setVisible@CCNode@cocos2d@@UEAAX_N@Z");
    if (!fn_setVisible) {
        cadence::log("hidepause: CCNode::setVisible not found, not loading");
        return;
    }

    cadence::hook<&UILayer::init>(hook_UILayer_init);
    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
