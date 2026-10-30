// Hitboxes — press H in a level to toggle the game's own debug hitbox drawing.
//
// PlayLayer::updateVisibility already redraws the hitboxes every frame while
// shouldDrawDebugHitboxes() is true, so all we do is answer "yes" and ask the
// game to sync the draw node's visibility once when the key is pressed.
#include <sdk.h>

static bool g_show = false;

static void(__fastcall* fn_toggleDebugDraw)(void* playLayer) = nullptr;
static bool(__fastcall* orig_shouldDraw)(void* self);
static bool(__fastcall* orig_shouldDrawBase)(void* self);
static void(__fastcall* orig_drawScene)(void* director);

static bool __fastcall hook_shouldDraw(void* self) {
    return g_show || orig_shouldDraw(self);
}

static bool __fastcall hook_shouldDrawBase(void* self) {
    return g_show || orig_shouldDrawBase(self);
}

static void __fastcall hook_drawScene(void* director) {
    void* pl = cadence::play_layer();
    if (pl && cadence::key_pressed('H')) {
        g_show = !g_show;
        if (fn_toggleDebugDraw) fn_toggleDebugDraw(pl);
    }
    orig_drawScene(director);
}

extern "C" __declspec(dllexport) void mod_unload() {
    // Hide the hitboxes we turned on; the hooks are removed by the loader.
    if (g_show) {
        g_show = false;
        void* pl = cadence::play_layer();
        if (pl && fn_toggleDebugDraw) fn_toggleDebugDraw(pl);
    }
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hitboxes","desc":"Toggle debug hitbox rendering","key":"H","ver":"1.1"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    fn_toggleDebugDraw = (decltype(fn_toggleDebugDraw))cadence::addr("PlayLayer::toggleDebugDraw()");

    cadence::hook_typed("PlayLayer::shouldDrawDebugHitboxes()", hook_shouldDraw, &orig_shouldDraw);
    cadence::hook_typed("GJBaseGameLayer::shouldDrawDebugHitboxes()", hook_shouldDrawBase, &orig_shouldDrawBase);

    cadence::hook(cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ"),
                  (void*)hook_drawScene, (void**)&orig_drawScene);
}
