// No glow — hides the glow/glitter effects in levels for cleaner visuals.
#include <sdk.h>
#include <windows.h>

static void(__fastcall* orig_toggleGlitter)(void* self, bool visible);

static void __fastcall hook_toggleGlitter(void* self, bool visible) {
    orig_toggleGlitter(self, false);
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::toggleGlitter(bool visible)",
        hook_toggleGlitter, &orig_toggleGlitter);
}
