// UI demo: press F7 to open a GD-style popup built with sdk_ui.h. Every action is
// written to loader.log, so a first run shows exactly which piece works.
#include <sdk_ui.h>

static bool g_checked = false;
static void(__fastcall* orig_drawScene)(void*);

static void open_demo() {
    cadence::log("uidemo: opening popup");
    bool ok = cadence::ui::Popup("UI demo", 320.0f, 200.0f)
        .label("Built with sdk_ui.h", 0.0f, 55.0f)
        .checkbox("A checkbox", -80.0f, 15.0f, g_checked, [](bool on) {
            g_checked = on;
            cadence::log("uidemo: checkbox is now %s", on ? "on" : "off");
        })
        .button("Alert", -75.0f, -40.0f, [] {
            cadence::log("uidemo: Alert button pressed");
            cadence::ui::alert("Hello", "This is a plain GD alert.");
        })
        .button("Confirm", 75.0f, -40.0f, [] {
            cadence::log("uidemo: Confirm button pressed");
            cadence::ui::confirm("Sure?", "Pick one.", "No", "Yes", [](bool yes) {
                cadence::log("uidemo: confirm answered %s", yes ? "Yes" : "No");
            });
        })
        .show();
    cadence::log("uidemo: popup %s", ok ? "shown" : "could not be built (see sdk_ui messages above)");
}

static void __fastcall hook_drawScene(void* director) {
    if (cadence::key_pressed(VK_F7)) open_demo();
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    void* draw_scene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (!cadence::hook(draw_scene, reinterpret_cast<void*>(hook_drawScene), reinterpret_cast<void**>(&orig_drawScene)))
        cadence::log("uidemo: could not hook drawScene, F7 will do nothing");
    else
        cadence::log("uidemo: ready, press F7");
}
