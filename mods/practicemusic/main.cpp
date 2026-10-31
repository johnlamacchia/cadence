// Practice music: play the level's normal music in practice mode.
// PlayLayer::startMusic picks the practice track when m_isPracticeMode is set,
// so clear the flag around the call and put it back.
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static void __fastcall hook_startMusic(PlayLayer* self) {
    bool practice = self->m_isPracticeMode;
    self->m_isPracticeMode = false;
    cadence::original<&PlayLayer::startMusic>(self);
    self->m_isPracticeMode = practice;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::startMusic>(hook_startMusic);
}
