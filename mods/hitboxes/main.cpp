// Hitbox viewer — press H to toggle debug hitbox rendering.
// Hooks shouldDrawDebugHitboxes + calls toggleDebugDraw/updateDebugDraw
// per-frame to actually render and keep hitboxes updated.
#include <sdk.h>
#include <windows.h>

static bool g_show_hitboxes = false;
static void* g_playlayer = nullptr;
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
    if (g_show_hitboxes) return true;
    return orig_shouldDraw(self);
}

static bool __fastcall hook_shouldDrawBase(void* self) {
    if (g_show_hitboxes) return true;
    return orig_shouldDrawBase(self);
}

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dc) {
    bool ret = orig_PlayLayer_init(self, level, dc);
    g_playlayer = self;
    g_draw_node_active = false;
    return ret;
}

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_playlayer = nullptr;
    g_draw_node_active = false;
    orig_PlayLayer_onQuit(self);
}

static void __fastcall hook_scheduler_update(void* scheduler, float dt) {
    orig_scheduler_update(scheduler, dt);

    if (!g_playlayer) return;

    if (g_show_hitboxes) {
        if (!g_draw_node_active && fn_toggleDebugDraw) {
            fn_toggleDebugDraw(g_playlayer);
            g_draw_node_active = true;
        }
        // Redraw hitboxes every frame so they track moving objects.
        // Clear the draw node, then call updateDebugDraw to repopulate.
        void* drawNode = *(void**)((char*)g_playlayer + 0x3870);
        if (drawNode && fn_drawNodeClear && fn_updateDebugDraw) {
            fn_drawNodeClear(drawNode);
            fn_updateDebugDraw(g_playlayer);
        }
    } else if (g_draw_node_active && fn_toggleDebugDraw) {
        fn_toggleDebugDraw(g_playlayer);
        g_draw_node_active = false;
    }
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('H') & 1) {
            g_show_hitboxes = !g_show_hitboxes;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_show_hitboxes = false;
    g_playlayer = nullptr;
    g_draw_node_active = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    fn_toggleDebugDraw = (toggleDebugDraw_t)cadence::addr("PlayLayer::toggleDebugDraw()");
    fn_updateDebugDraw = (updateDebugDraw_t)cadence::addr("GJBaseGameLayer::updateDebugDraw()");

    HMODULE cocos = GetModuleHandleW(L"libcocos2d.dll");
    if (cocos) {
        fn_drawNodeClear = (CCDrawNode_clear_t)GetProcAddress(cocos,
            "?clear@CCDrawNode@cocos2d@@QEAAXXZ");
    }

    cadence::hook_typed("PlayLayer::shouldDrawDebugHitboxes()",
        hook_shouldDraw, &orig_shouldDraw);
    cadence::hook_typed("GJBaseGameLayer::shouldDrawDebugHitboxes()",
        hook_shouldDrawBase, &orig_shouldDrawBase);
    cadence::hook_typed("PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);
    cadence::hook_typed("PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    if (cocos) {
        void* schedUpdate = (void*)GetProcAddress(cocos,
            "?update@CCScheduler@cocos2d@@UEAAXM@Z");
        if (schedUpdate) {
            cadence::hook(schedUpdate, (void*)hook_scheduler_update,
                (void**)&orig_scheduler_update);
        }
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
