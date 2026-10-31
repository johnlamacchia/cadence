// Hide attempts: the in-level "Attempt N" label is always hidden.
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static void __fastcall hook_toggleHideAttempts(PlayLayer* self, bool /*hide*/) {
    cadence::original<&PlayLayer::toggleHideAttempts>(self, true);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::toggleHideAttempts>(hook_toggleHideAttempts);
}
