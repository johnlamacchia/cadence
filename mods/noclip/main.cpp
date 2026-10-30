// Noclip mod — press N to toggle noclip (prevents player death).
#include <sdk.h>
#include <windows.h>

static bool g_noclip = false;
static bool g_in_level = false;

// PlayLayer::destroyPlayer(PlayerObject*, GameObject*)
static void(__fastcall* orig_destroyPlayer)(void* self, void* player, void* object);

static void __fastcall hook_destroyPlayer(void* self, void* player, void* object) {
    if (g_noclip) return;
    orig_destroyPlayer(self, player, object);
}

// PlayLayer::init to track when we're in a level
static bool(__fastcall* orig_PlayLayer_init)(void* self, void* level, bool dontCreate);

static bool __fastcall hook_PlayLayer_init(void* self, void* level, bool dontCreate) {
    g_in_level = true;
    return orig_PlayLayer_init(self, level, dontCreate);
}

// PlayLayer::onQuit to track when we leave
static void(__fastcall* orig_PlayLayer_onQuit)(void* self);

static void __fastcall hook_PlayLayer_onQuit(void* self) {
    g_in_level = false;
    orig_PlayLayer_onQuit(self);
}

static DWORD WINAPI key_thread(LPVOID) {
    while (true) {
        if (GetAsyncKeyState('N') & 1) {
            g_noclip = !g_noclip;
        }
        Sleep(50);
    }
    return 0;
}

extern "C" __declspec(dllexport) void mod_unload() {
    g_in_level = false;
    g_noclip = false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::destroyPlayer(PlayerObject* player, GameObject* object)",
        hook_destroyPlayer, &orig_destroyPlayer);

    cadence::hook_typed(
        "PlayLayer::init(GJGameLevel* level, bool dontCreateObjects)",
        hook_PlayLayer_init, &orig_PlayLayer_init);

    cadence::hook_typed(
        "PlayLayer::onQuit()",
        hook_PlayLayer_onQuit, &orig_PlayLayer_onQuit);

    CreateThread(nullptr, 0, key_thread, nullptr, 0, nullptr);
}
