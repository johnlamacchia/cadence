// Hitbox viewer — press H to toggle debug hitbox rendering.
// Hooks shouldDrawDebugHitboxes + calls toggleDebugDraw/updateDebugDraw
// per-frame to actually render and keep hitboxes updated.
#include <sdk.h>
#include <windows.h>

static bool g_show_hitboxes = false;
static void* g_playlayer = nullptr;
static volatile bool g_in_level = false;
static bool g_draw_node_active = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

using toggleDebugDraw_t = void(__fastcall*)(void*);
using updateDebugDraw_t = void(__fastcall*)(void*);
using CCDrawNode_clear_t = void(__fastcall*)(void*);

static toggleDebugDraw_t fn_toggleDebugDraw = nullptr;
static updateDebugDraw_t fn_updateDebugDraw = nullptr;
static CCDrawNode_clear_t fn_drawNodeClear = nullptr;

static bool(__fastcall* orig_shouldDraw)(void* self);
static bool(__fastcall* orig_shouldDrawBase)(void* self);
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dc);
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);
static void(__fastcall* orig_scheduler_update)(void*, float);

static bool __fastcall hook_shouldDraw(void* self) {
    return g_show_hitboxes && g_in_level;
}

static bool __fastcall hook_shouldDrawBase(void* self) {
    return g_show_hitboxes && g_in_level;
}

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dc) {
    bool ret = orig_PlayLayer_init(self, level, dc);
    g_draw_node_active = false;
    g_playlayer = self;
    g_in_level = true;
    return ret;
}

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    if (g_draw_node_active) {
        // Clear the draw node geometry so nothing renders during transition
        void* drawNode = *(void**)((char*)self + 0x3870);
        uintptr_t dn = (uintptr_t)drawNode;
        if (drawNode && dn > 0x10000 && dn < 0x7FFFFFFFFFFF && fn_drawNodeClear) {
            fn_drawNodeClear(drawNode);
        }
        // Toggle debug draw OFF in the engine
        if (fn_toggleDebugDraw) fn_toggleDebugDraw(self);
    }
    // Kill all state BEFORE calling original — shouldDrawDebugHitboxes
    // will return false for any further calls during the quit sequence
    g_show_hitboxes = false;
    g_in_level = false;
    g_draw_node_active = false;
    g_playlayer = nullptr;
    orig_PlayLayer_onQuit(self);
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (!g_in_level || !g_playlayer) return;

    __try {
        void* pl = g_playlayer;
        if (!pl) return;

        if (g_show_hitboxes) {
            if (!g_draw_node_active && fn_toggleDebugDraw) {
                fn_toggleDebugDraw(pl);
                if (!g_in_level) return;
                g_draw_node_active = true;
            }
            if (!g_in_level || !g_playlayer) return;
            void* drawNode = *(void**)((char*)pl + 0x3870);
            uintptr_t dn = (uintptr_t)drawNode;
            if (drawNode && dn > 0x10000 && dn < 0x7FFFFFFFFFFF &&
                fn_drawNodeClear && fn_updateDebugDraw) {
                fn_drawNodeClear(drawNode);
                if (g_in_level) fn_updateDebugDraw(pl);
            }
        } else if (g_draw_node_active && fn_toggleDebugDraw) {
            fn_toggleDebugDraw(pl);
            g_draw_node_active = false;
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        g_draw_node_active = false;
    }
}

static DWORD WINAPI key_thread(LPVOID) {
    bool was_down = false;
    while (g_running) {
        bool is_down = (GetAsyncKeyState('H') & 0x8000) != 0;
        if (is_down && !was_down) {
            g_show_hitboxes = !g_show_hitboxes;
        }
        was_down = is_down;
        Sleep(30);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_show_hitboxes = false;
    g_in_level = false;
    g_playlayer = nullptr;
    g_draw_node_active = false;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hitboxes","desc":"Toggle debug hitbox rendering","key":"H","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    fn_toggleDebugDraw = (toggleDebugDraw_t)cadence::addr("PlayLayer::toggleDebugDraw()");
    fn_updateDebugDraw = (updateDebugDraw_t)cadence::addr("GJBaseGameLayer::updateDebugDraw()");

    fn_drawNodeClear = (CCDrawNode_clear_t)cadence::cocos("?clear@CCDrawNode@cocos2d@@QEAAXXZ");

    cadence::hook_typed("PlayLayer::shouldDrawDebugHitboxes()",
        hook_shouldDraw, &orig_shouldDraw);
    cadence::hook_typed("GJBaseGameLayer::shouldDrawDebugHitboxes()",
        hook_shouldDrawBase, &orig_shouldDrawBase);
    cadence::hook_typed("PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);
    cadence::hook_typed("PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    void* schedUpdate = cadence::cocos("?update@CCScheduler@cocos2d@@UEAAXM@Z");
    if (schedUpdate) {
        cadence::hook(schedUpdate, (void*)hook_scheduler_update,
            (void**)&orig_scheduler_update);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
