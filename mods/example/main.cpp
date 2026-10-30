// Example mod: adds a "Hello" button to the GD main menu.
// When clicked, shows an FLAlertLayer dialog.
#include <sdk.h>
#include <windows.h>
#include <cstdio>
#include <string>

// Minimal cocos2d types (just enough to compile)
namespace cocos2d {
    struct CCObject { virtual ~CCObject(); };
    struct CCNode : CCObject {};
    struct CCLayer : CCNode {};
    struct CCScene : CCNode {};
    struct CCMenu : CCNode {
        // cocos2d::CCMenu::addChild is exported from libcocos2d.dll
    };
    struct CCPoint { float x, y; };
    struct CCSize { float width, height; };
    struct CCLabelBMFont : CCNode {};
    struct CCSprite : CCNode {};
    struct SEL_MenuHandler {};
}

// Forward declarations for GD classes we use
class MenuLayer;
class FLAlertLayer;
class CCMenuItemSpriteExtra;
class ButtonSprite;

// Function pointer types
// std::string by value on MSVC x64 is passed as a hidden pointer
using FLAlertLayer_create_t = FLAlertLayer* (*)(
    void* delegate, const char* title, std::string* desc,
    const char* btn1, const char* btn2, float width);

using ButtonSprite_create_t = ButtonSprite* (*)(
    const char* caption, int width, int minWidth, float scale,
    bool absolute, const char* font, const char* bg, float height);

using CCMenuItemSpriteExtra_create_t = CCMenuItemSpriteExtra* (*)(
    cocos2d::CCNode* sprite, cocos2d::CCNode* disabledSprite,
    cocos2d::CCObject* target, void* selector);

// Resolved function pointers (filled in mod_main)
static FLAlertLayer_create_t g_FLAlertLayer_create = nullptr;
static ButtonSprite_create_t g_ButtonSprite_create = nullptr;
static CCMenuItemSpriteExtra_create_t g_CCMenuItemSpriteExtra_create = nullptr;
static void* g_FLAlertLayer_show = nullptr;

// Hook originals
static bool(__fastcall* orig_MenuLayer_init)(MenuLayer* self);

// Our callback for the button
static void __fastcall on_hello_button(cocos2d::CCObject* self, cocos2d::CCObject* sender) {
    if (!g_FLAlertLayer_create) return;
    std::string desc("Hello from Cadence!\n\nThis button was added by the example mod.");
    auto* alert = g_FLAlertLayer_create(
        nullptr,                     // no delegate
        "Cadence",                   // title
        &desc,                       // std::string passed by hidden pointer
        "OK",                        // btn1
        nullptr,                     // btn2
        300.0f                       // width
    );
    if (alert && g_FLAlertLayer_show) {
        // Call alert->show() via the resolved address
        using show_t = void(__fastcall*)(FLAlertLayer*);
        ((show_t)g_FLAlertLayer_show)(alert);
    }
}

// Hooked MenuLayer::init
static bool __fastcall hook_MenuLayer_init(MenuLayer* self) {
    if (!orig_MenuLayer_init(self)) return false;

    // Create a "Hello" button and add it to the menu
    // ButtonSprite::create("Hello", ...) → CCMenuItemSpriteExtra::create(sprite, ...)
    if (g_ButtonSprite_create && g_CCMenuItemSpriteExtra_create) {
        auto* label = g_ButtonSprite_create("Hello", 0, 0, 1.0f, false, "bigFont.fnt", "GJ_button_01.png", 30.0f);
        if (label) {
            auto* btn = g_CCMenuItemSpriteExtra_create(
                (cocos2d::CCNode*)label,
                nullptr,
                (cocos2d::CCObject*)self,
                (void*)&on_hello_button
            );
            if (btn) {
                // Get the bottom menu from MenuLayer
                // MenuLayer inherits CCLayer which has getChildByTag etc.
                // We'll add our button to the existing menu by finding it
                // For simplicity, use cocos2d functions from libcocos2d.dll
                using addChild_t = void(__fastcall*)(cocos2d::CCNode*, cocos2d::CCNode*);
                using setPosition_t = void(__fastcall*)(cocos2d::CCNode*, float, float);

                {
                    auto addChild = (addChild_t)cadence::cocos(
                        "?addChild@CCNode@cocos2d@@UEAAXPEAVCCLayer@2@@Z");
                    if (!addChild) addChild = (addChild_t)cadence::cocos(
                        "?addChild@CCNode@cocos2d@@UEAAXPEAV12@@Z");

                    auto setPos = (setPosition_t)cadence::cocos(
                        "?setPosition@CCNode@cocos2d@@UEAAXMM@Z");

                    using CCMenu_create_t = cocos2d::CCMenu*(__cdecl*)();
                    auto ccMenuCreate = (CCMenu_create_t)cadence::cocos(
                        "?create@CCMenu@cocos2d@@SAPEAV12@XZ");

                    if (ccMenuCreate && addChild) {
                        auto* menu = ccMenuCreate();
                        if (menu) {
                            addChild((cocos2d::CCNode*)menu, (cocos2d::CCNode*)btn);
                            addChild((cocos2d::CCNode*)self, (cocos2d::CCNode*)menu);
                            if (setPos) {
                                setPos((cocos2d::CCNode*)btn, -200.0f, -120.0f);
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

extern "C" __declspec(dllexport) void mod_unload() {
    void* init_addr = cadence::addr("MenuLayer::init()");
    if (init_addr) cadence::unhook(init_addr);
    orig_MenuLayer_init = nullptr;
    g_FLAlertLayer_create = nullptr;
    g_FLAlertLayer_show = nullptr;
    g_ButtonSprite_create = nullptr;
    g_CCMenuItemSpriteExtra_create = nullptr;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Example","desc":"Adds a Hello button to the main menu","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    // Resolve GD functions
    g_FLAlertLayer_create = (FLAlertLayer_create_t)cadence::addr(
        "FLAlertLayer::create(FLAlertLayerProtocol* delegate, char const* title, std::string desc, char const* btn1, char const* btn2, float width)");

    g_FLAlertLayer_show = cadence::addr("FLAlertLayer::show()");

    g_ButtonSprite_create = (ButtonSprite_create_t)cadence::addr(
        "ButtonSprite::create(char const* caption, int width, int minWidth, float scale, bool absolute, char const* font, char const* bg, float height)");

    g_CCMenuItemSpriteExtra_create = (CCMenuItemSpriteExtra_create_t)cadence::addr(
        "CCMenuItemSpriteExtra::create(cocos2d::CCNode* sprite, cocos2d::CCNode* selectedSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback)");

    // Hook MenuLayer::init
    void* init_addr = cadence::addr("MenuLayer::init()");
    if (init_addr) {
        cadence::hook(init_addr, (void*)hook_MenuLayer_init, (void**)&orig_MenuLayer_init);
    }
}
