// No glow: the glitter effect is always off.
#include <sdk.h>
#include <gd/PlayLayer.hpp>

static void __fastcall hook_toggleGlitter(PlayLayer* self, bool /*visible*/) {
    cadence::original<&PlayLayer::toggleGlitter>(self, false);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&PlayLayer::toggleGlitter>(hook_toggleGlitter);
}
