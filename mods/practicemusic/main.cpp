// Practice music hack — plays normal level music in practice mode
// instead of the practice mode loop.
// Hooks PlayLayer::startMusic, temporarily clears m_isPracticeMode so
// the original plays normal music, then restores the flag.
#include <sdk.h>
#include <windows.h>

static void(__fastcall* orig_startMusic)(void* self);
// GJBaseGameLayer::m_isPracticeMode offset from generated Broma headers (GD 2.2091)
static constexpr size_t PRACTICE_MODE_OFFSET = 0x31E8;

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
