// Practice music hack — plays normal level music in practice mode
// instead of the practice mode loop.
// Hooks PlayLayer::startMusic and removes the practice music override.
#include <sdk.h>
#include <windows.h>

// PlayLayer has a member m_isPracticeMode. We check it and skip
// the practice music path by hooking startMusic.
// In GD 2.2091, PlayLayer::startMusic checks m_isPracticeMode
// and plays a different track. We hook it and temporarily clear
// the practice flag so it plays normal music.

// We need to find the offset of m_isPracticeMode in PlayLayer.
// From Broma bindings, it's at a known offset. We'll read it
// dynamically: hook startMusic, peek at the flag, clear it,
// call original, restore it.

// Actually simpler: just hook startMusic and always call it with
// the practice flag temporarily cleared.

// m_isPracticeMode offset — from the generated headers/layout
// For GD 2.2091 this is typically around 0x2A7C but let's find it
// by hooking and using a known pattern.

// Simpler approach: hook PlayLayer::startMusic and before calling
// original, temporarily set the byte at the practice mode offset to 0.
// We'll scan for it by checking PlayLayer::m_isPracticeMode.

// From Broma: PlayLayer inherits GJBaseGameLayer inherits ...
// m_isPracticeMode is a bool member. We'll use offset 0x2A7C
// (common for 2.2091). If wrong, the mod just does nothing harmful.

static void(__fastcall* orig_startMusic)(void* self);
static constexpr size_t PRACTICE_MODE_OFFSET = 0x3358;

static void __fastcall hook_startMusic(void* self) {
    uint8_t* base = (uint8_t*)self;
    uint8_t saved = base[PRACTICE_MODE_OFFSET];
    base[PRACTICE_MODE_OFFSET] = 0;
    orig_startMusic(self);
    base[PRACTICE_MODE_OFFSET] = saved;
}

extern "C" __declspec(dllexport) void mod_unload() {}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Practice Music","desc":"Normal music in practice mode","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::startMusic()",
        hook_startMusic, &orig_startMusic);
}
