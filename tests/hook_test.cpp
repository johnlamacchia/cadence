// Hook engine tests: chaining, removal from any position, full restore and
// re-hooking. Runs standalone (no GD needed): `ctest -C Release` after a build.
#include "hook.h"
#include <windows.h>
#include <cstdio>
#include <cstring>

static volatile int g_zero = 0;  // forces a RIP-relative load in the hooked prologue

#pragma optimize("", off)
__declspec(noinline) int target_fn(int x) {
    int a = x * 3;
    int b = a + 7 + g_zero;
    for (int i = 0; i < 4; i++) b += g_zero;
    return b;
}
#pragma optimize("", on)

static int (*volatile call_target)(int) = target_fn;

using fn_t = int(*)(int);

static fn_t slot_a, slot_b, slot_c;
static int detour_a(int x) { return slot_a(x) + 1000; }
static int detour_b(int x) { return slot_b(x) + 20000; }
static int detour_c(int x) { return slot_c(x) + 300000; }

static int g_failed = 0;
#define CHECK(cond, ...) do { if (!(cond)) { g_failed++; printf("FAIL line %d: ", __LINE__); printf(__VA_ARGS__); printf("\n"); } } while (0)

static void remove_one(void* detour) {
    int n = hook::remove_in_range((void*)target_fn, (uintptr_t)detour, (uintptr_t)detour + 1);
    CHECK(n == 1, "expected to remove 1 hook, removed %d", n);
}

int main() {
    uint8_t original_bytes[32];
    memcpy(original_bytes, (void*)target_fn, sizeof(original_bytes));

    const int base = call_target(5);
    CHECK(base == 22, "unhooked target returns %d, expected 22", base);

    // Chain of three hooks, newest runs first.
    CHECK(hook::install((void*)target_fn, (void*)detour_a, (void**)&slot_a), "install A");
    CHECK(call_target(5) == base + 1000, "A only: got %d", call_target(5));
    CHECK(hook::install((void*)target_fn, (void*)detour_b, (void**)&slot_b), "install B");
    CHECK(hook::install((void*)target_fn, (void*)detour_c, (void**)&slot_c), "install C");
    CHECK(call_target(5) == base + 321000, "A+B+C: got %d", call_target(5));

    // Remove the middle hook: C must now call A directly.
    remove_one((void*)detour_b);
    CHECK(call_target(5) == base + 301000, "after removing B: got %d", call_target(5));
    CHECK(slot_c == detour_a, "C's original was not relinked to A");

    // Remove the head: A becomes the entry point.
    remove_one((void*)detour_c);
    CHECK(call_target(5) == base + 1000, "after removing C: got %d", call_target(5));

    // Remove the last hook: the game's own bytes must be back, byte for byte.
    remove_one((void*)detour_a);
    CHECK(call_target(5) == base, "fully unhooked: got %d", call_target(5));
    CHECK(memcmp(original_bytes, (void*)target_fn, sizeof(original_bytes)) == 0,
          "original bytes were not restored exactly");

    // Hook again after a full unhook (hot reload does this): trampoline is reused.
    CHECK(hook::install((void*)target_fn, (void*)detour_b, (void**)&slot_b), "re-install B");
    CHECK(hook::install((void*)target_fn, (void*)detour_a, (void**)&slot_a), "re-install A");
    CHECK(call_target(5) == base + 21000, "re-hooked B+A: got %d", call_target(5));

    // Remove the OLDEST hook while a newer one stays: A must fall through to the real code.
    remove_one((void*)detour_b);
    CHECK(call_target(5) == base + 1000, "after removing oldest: got %d", call_target(5));

    // Range removal across a whole "DLL".
    int n = hook::remove_by_detour_range((uintptr_t)detour_a, (uintptr_t)detour_a + 1);
    CHECK(n == 1, "range removal removed %d", n);
    CHECK(call_target(5) == base, "after range removal: got %d", call_target(5));

    printf(g_failed ? "hook_test: %d FAILED\n" : "hook_test: all passed\n", g_failed);
    return g_failed ? 1 : 0;
}
