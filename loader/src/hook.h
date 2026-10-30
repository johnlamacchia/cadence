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
void shutdown();

}
