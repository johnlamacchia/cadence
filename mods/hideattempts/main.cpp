// Hide attempts — removes the attempt counter label in the top-left of levels.
#include <sdk.h>
#include <windows.h>

static void(__fastcall* orig_toggleHideAttempts)(void* self, bool hide);

static void __fastcall hook_toggleHideAttempts(void* self, bool hide) {
    orig_toggleHideAttempts(self, true);
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Hide Attempts","desc":"Hides the attempt counter label","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "PlayLayer::toggleHideAttempts(bool hide)",
        hook_toggleHideAttempts, &orig_toggleHideAttempts);
}
