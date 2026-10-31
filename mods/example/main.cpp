// Example mod: clicking the RobTop logo on the main menu shows "Hello" in GD's own
// alert dialog, built with FLAlertLayer::create.
//
// This is the minimal shape of a mod: include the headers for the classes you use,
// hook a function with its real signature, call another game function by its
// real signature.
//
// Why onRobTop and not onMoreGames: the bindings give MenuLayer::onMoreGames no
// Windows address in 2.2091 (the button no longer exists on Windows), so it cannot
// be hooked. `cadence::hook<&MenuLayer::onMoreGames>` would fail to compile with
// an error saying exactly that.
#include <sdk.h>
#include <gd/MenuLayer.hpp>
#include <gd/FLAlertLayer.hpp>

// FLAlertLayer::create is overloaded, so name the overload we want.
constexpr auto alert_create = static_cast<FLAlertLayer* (*)(FLAlertLayerProtocol*, char const*, std::string, char const*, char const*, float)>(&FLAlertLayer::create);

static void __fastcall hook_onRobTop(MenuLayer* self, cocos2d::CCObject* sender) {
    auto create = cadence::fn<alert_create>();
    auto show = cadence::fn<&FLAlertLayer::show>();
    if (!create || !show) {
        cadence::original<&MenuLayer::onRobTop>(self, sender);   // addresses missing: behave like stock GD
        return;
    }
    FLAlertLayer* alert = create(nullptr, "Cadence", "Hello from a Cadence mod!", "OK", nullptr, 300.0f);
    show(alert);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&MenuLayer::onRobTop>(hook_onRobTop);
}
