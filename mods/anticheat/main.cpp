// Anti-cheat bypass — disables GD's built-in spawn abuse check.
// Hooks GJBaseGameLayer::checkSpawnAbuse to always succeed.
#include <sdk.h>
#include <windows.h>

static void(__fastcall* orig_checkSpawnAbuse)(void* self);

static void __fastcall hook_checkSpawnAbuse(void* self) {
    // Skip the original spawn abuse check entirely.
}

extern "C" __declspec(dllexport) const char* mod_info() {
    return R"({"name":"Anti-Cheat Bypass","desc":"Bypasses spawn abuse check","ver":"1.0"})";
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);

    cadence::hook_typed(
        "GJBaseGameLayer::checkSpawnAbuse()",
        hook_checkSpawnAbuse, &orig_checkSpawnAbuse);
}
