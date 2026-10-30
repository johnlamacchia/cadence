// Hide ground — removes ground and middle-ground lines for a clean gameplay view.
// Press V to toggle ground visibility (works live during gameplay).
// All game function calls happen on the main thread via scheduler hook
// to avoid crashes when the level is exited.
#include <sdk.h>
#include <windows.h>

static bool g_hide = false;
static void* g_playlayer = nullptr;
static volatile bool g_toggle_pending = false;
static volatile bool g_running = true;
static HANDLE g_thread = nullptr;

static void(__fastcall* orig_toggleGroundVisibility)(void* self, bool visible);
static void(__fastcall* orig_toggleMGVisibility)(void* self, bool visible);
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dc);
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);
static void(__fastcall* orig_drawScene)(void*);

static void __fastcall hook_toggleGroundVisibility(void* self, bool visible) {
    if (g_hide) {
        orig_toggleGroundVisibility(self, false);
        return;
    }
    orig_toggleGroundVisibility(self, visible);
}

static void __fastcall hook_toggleMGVisibility(void* self, bool visible) {
    if (g_hide) {
        orig_toggleMGVisibility(self, false);
        return;
    }
    orig_toggleMGVisibility(self, visible);
}

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dc) {
    bool ret = orig_PlayLayer_init(self, level, dc);
    g_playlayer = self;
    return ret;
}

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_playlayer = nullptr;
    orig_PlayLayer_onQuit(self);
}

static void __fastcall hook_drawScene(void* director) {
    if (g_toggle_pending && g_playlayer) {
        g_toggle_pending = false;
        orig_toggleGroundVisibility(g_playlayer, !g_hide);
        orig_toggleMGVisibility(g_playlayer, !g_hide);
    }
    orig_drawScene(director);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (g_running) {
        if (GetAsyncKeyState('V') & 1) {
            g_hide = !g_hide;
            g_toggle_pending = true;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_running = false;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = nullptr; }
    g_hide = false;
    g_playlayer = nullptr;
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hide Ground","desc":"Hides ground and mid-ground lines","key":"V","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    g_running = true;

    cadence::hook_typed(
        "PlayLayer::toggleGroundVisibility(bool visible)",
        hook_toggleGroundVisibility, &orig_toggleGroundVisibility);

    cadence::hook_typed(
        "PlayLayer::toggleMGVisibility(bool visible)",
        hook_toggleMGVisibility, &orig_toggleMGVisibility);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    void* drawScene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (drawScene) {
        cadence::hook(drawScene, (void*)hook_drawScene,
            (void**)&orig_drawScene);
    }

    g_thread = CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
