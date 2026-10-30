#pragma once
#include <cstdint>

namespace hook {

// Install a hook. *original receives the next function in the chain: the
// previously installed detour for this target, or a trampoline to the real code.
bool install(void* target, void* detour, void** original);

// Remove every hook on `target` whose detour lies in [lo, hi).
// Chains are relinked: hooks installed after a removed one are re-pointed at
// whatever the removed hook was calling, so nothing dangles.
int remove_in_range(void* target, uintptr_t lo, uintptr_t hi);

// Remove every hook, on any target, whose detour lies in [lo, hi).
int remove_by_detour_range(uintptr_t lo, uintptr_t hi);

}
