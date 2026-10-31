// Anti-cheat bypass: GJBaseGameLayer::checkSpawnAbuse() is a getter for a flag
// the game sets when it decides a level abuses spawn triggers. Always answer no.
#include <sdk.h>
#include <gd/GJBaseGameLayer.hpp>

static bool __fastcall hook_checkSpawnAbuse(GJBaseGameLayer*) {
    return false;
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    cadence::hook<&GJBaseGameLayer::checkSpawnAbuse>(hook_checkSpawnAbuse);
}
