#pragma once
#include <cstdint>

namespace hook {

struct HookEntry {
    void* target;
    void* detour;
    void* trampoline;
    HookEntry* next;
};

bool install(void* target, void* detour, void** original);
bool remove(void* target);
int remove_by_detour_range(uintptr_t range_start, uintptr_t range_end);
void shutdown();

}
