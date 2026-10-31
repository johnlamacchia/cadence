#pragma once
#include <windows.h>

namespace loader {
    bool init();
    void reload_mods();
    void claim_input(bool claim);     // a mod reading typed text: see cadence_claim_input
    bool input_claimed();
}
