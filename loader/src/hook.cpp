#include "hook.h"
#include <windows.h>
#include <Zydis/Zydis.h>
#include <cstring>
#include <memory>
#include <mutex>
#include <vector>

namespace hook {

static constexpr size_t JMP_ABS_SIZE = 14;   // ff 25 00000000 + 8-byte addr
static constexpr size_t TRAMPOLINE_MAX = 128;
static constexpr size_t SAVED_MAX = 32;

// One hook in a target's chain. `next` is what this hook's `original` points
// at: the previous (older) detour, or the trampoline for the oldest hook.
// `slot` is the mod's own `original` variable, so we can re-point it when the
// hook it was calling is removed.
struct Link {
    void* detour;
    void** slot;
    void* next;
};

// Everything we know about one hooked function. The trampoline and the saved
// original bytes live as long as the process, so a target can be hooked,
// fully unhooked and hooked again (hot reload) without reallocating.
struct Target {
    void* target;
    uint8_t saved[SAVED_MAX];
    size_t patch_size;
    uint8_t* trampoline;
    std::vector<Link> chain;   // [0] oldest ... back() newest, the one the target jumps to
};

// Targets are never freed (see Target), so a pointer into this list stays valid.
static std::vector<std::unique_ptr<Target>> g_targets;
static std::mutex g_lock;

static void write_abs_jmp(uint8_t* buf, void* target) {
    buf[0] = 0xFF;
    buf[1] = 0x25;
    *(uint32_t*)(buf + 2) = 0;
    *(uint64_t*)(buf + 6) = (uint64_t)target;
}

static void* alloc_near(void* target, size_t size) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    uintptr_t addr = (uintptr_t)target;
    uintptr_t min_addr = addr > 0x7FFF0000 ? addr - 0x7FFF0000 : (uintptr_t)si.lpMinimumApplicationAddress;
    uintptr_t max_addr = addr + 0x7FFF0000;
    if (max_addr > (uintptr_t)si.lpMaximumApplicationAddress)
        max_addr = (uintptr_t)si.lpMaximumApplicationAddress;

    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t search = min_addr;
    while (search < max_addr) {
        if (VirtualQuery((void*)search, &mbi, sizeof(mbi)) == 0) break;
        if (mbi.State == MEM_FREE && mbi.RegionSize >= size) {
            void* p = VirtualAlloc((void*)search, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
            if (p) return p;
        }
        search = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    }
    return VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

static bool relocate_instruction(
    const ZydisDecodedInstruction* insn,
    const ZydisDecodedOperand* operands,
    uint64_t orig_addr,
    uint8_t* dest,
    uint64_t dest_addr,
    size_t* out_size
) {
    bool has_rip_rel = false;
    uint64_t abs_target = 0;

    for (int i = 0; i < insn->operand_count_visible; i++) {
        if (operands[i].type == ZYDIS_OPERAND_TYPE_MEMORY &&
            operands[i].mem.base == ZYDIS_REGISTER_RIP) {
            has_rip_rel = true;
            ZydisCalcAbsoluteAddress(insn, &operands[i], orig_addr, &abs_target);
            break;
        }
        if (operands[i].type == ZYDIS_OPERAND_TYPE_IMMEDIATE &&
            operands[i].imm.is_relative) {
            has_rip_rel = true;
            ZydisCalcAbsoluteAddress(insn, &operands[i], orig_addr, &abs_target);
            break;
        }
    }

    if (!has_rip_rel) {
        memcpy(dest, (void*)orig_addr, insn->length);
        *out_size = insn->length;
        return true;
    }

    if (operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE && operands[0].imm.is_relative) {
        // Relative branch: jmp/jcc/call with RIP-relative offset
        if (insn->mnemonic == ZYDIS_MNEMONIC_JMP) {
            write_abs_jmp(dest, (void*)abs_target);
            *out_size = JMP_ABS_SIZE;
            return true;
        }
        if (insn->mnemonic == ZYDIS_MNEMONIC_CALL) {
            // The trampoline is allocated within +/-2GB, so a rel32 call still reaches.
            int64_t new_disp = (int64_t)abs_target - (int64_t)(dest_addr + 5);
            if (new_disp >= INT32_MIN && new_disp <= INT32_MAX) {
                dest[0] = 0xE8;
                *(int32_t*)(dest + 1) = (int32_t)new_disp;
                *out_size = 5;
                return true;
            }
            return false;
        }
        // Conditional jumps: inverted short jcc over an absolute jmp.
        uint8_t cond = insn->opcode & 0x0F;
        dest[0] = 0x70 | (cond ^ 1);
        dest[1] = JMP_ABS_SIZE;
        write_abs_jmp(dest + 2, (void*)abs_target);
        *out_size = 2 + JMP_ABS_SIZE;
        return true;
    }

    // RIP-relative memory operand: copy and fix the displacement.
    memcpy(dest, (void*)orig_addr, insn->length);
    int64_t new_disp = (int64_t)abs_target - (int64_t)(dest_addr + insn->length);
    if (new_disp < INT32_MIN || new_disp > INT32_MAX) {
        return false;
    }
    if (insn->raw.disp.offset > 0) {
        *(int32_t*)(dest + insn->raw.disp.offset) = (int32_t)new_disp;
    }
    *out_size = insn->length;
    return true;
}

static Target* find_target(void* target) {
    for (auto& t : g_targets)
        if (t->target == target) return t.get();
    return nullptr;
}

// Decode the prologue, build the trampoline and remember the original bytes.
// Does not patch the target; the caller does that once a detour is known.
static Target* create_target(void* target) {
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    uint8_t* func = (uint8_t*)target;
    size_t copied = 0;
    ZydisDecodedInstruction insn;
    ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
    while (copied < JMP_ABS_SIZE) {
        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, func + copied, 15, &insn, operands)))
            return nullptr;
        copied += insn.length;
    }
    if (copied > SAVED_MAX) return nullptr;

    uint8_t* tramp = (uint8_t*)alloc_near(target, TRAMPOLINE_MAX);
    if (!tramp) return nullptr;

    size_t tramp_offset = 0;
    size_t src_offset = 0;
    while (src_offset < copied) {
        ZydisDecoderDecodeFull(&decoder, func + src_offset, 15, &insn, operands);
        // Worst case one instruction expands to 16 bytes, plus the final jump back.
        if (tramp_offset + 16 + JMP_ABS_SIZE > TRAMPOLINE_MAX) {
            VirtualFree(tramp, 0, MEM_RELEASE);
            return nullptr;
        }
        size_t relocated_size;
        if (!relocate_instruction(&insn, operands, (uint64_t)(func + src_offset),
                                  tramp + tramp_offset, (uint64_t)(tramp + tramp_offset),
                                  &relocated_size)) {
            VirtualFree(tramp, 0, MEM_RELEASE);
            return nullptr;
        }
        src_offset += insn.length;
        tramp_offset += relocated_size;
    }
    write_abs_jmp(tramp + tramp_offset, func + copied);
    FlushInstructionCache(GetCurrentProcess(), tramp, TRAMPOLINE_MAX);

    auto t = std::make_unique<Target>();
    t->target = target;
    memcpy(t->saved, func, copied);
    t->patch_size = copied;
    t->trampoline = tramp;
    g_targets.push_back(std::move(t));
    return g_targets.back().get();
}

static void patch_jmp(Target* t, void* dest) {
    uint8_t* func = (uint8_t*)t->target;
    DWORD old_protect;
    VirtualProtect(func, t->patch_size, PAGE_EXECUTE_READWRITE, &old_protect);
    write_abs_jmp(func, dest);
    for (size_t i = JMP_ABS_SIZE; i < t->patch_size; i++) func[i] = 0x90;
    VirtualProtect(func, t->patch_size, old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), func, t->patch_size);
}

static void restore_original(Target* t) {
    uint8_t* func = (uint8_t*)t->target;
    DWORD old_protect;
    VirtualProtect(func, t->patch_size, PAGE_EXECUTE_READWRITE, &old_protect);
    memcpy(func, t->saved, t->patch_size);
    VirtualProtect(func, t->patch_size, old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), func, t->patch_size);
}

bool install(void* target, void* detour, void** original) {
    if (!target || !detour || !original) return false;
    std::lock_guard lock(g_lock);

    Target* t = find_target(target);
    if (!t) t = create_target(target);
    if (!t) return false;

    void* next = t->chain.empty() ? static_cast<void*>(t->trampoline) : t->chain.back().detour;
    t->chain.push_back(Link{detour, original, next});
    *original = next;
    patch_jmp(t, detour);
    return true;
}

// Unlink chain[idx] and fix up whatever pointed at it.
static void remove_link(Target* t, size_t idx) {
    Link removed = t->chain[idx];
    bool is_head = idx + 1 == t->chain.size();

    if (is_head) {
        if (idx == 0) restore_original(t);              // last hook: put the game's bytes back
        else patch_jmp(t, t->chain[idx - 1].detour);    // older hook becomes the entry point
    } else {
        // The next-newer hook was calling us; make it call what we were calling.
        Link& newer = t->chain[idx + 1];
        newer.next = removed.next;
        if (newer.slot) *newer.slot = removed.next;
    }
    t->chain.erase(t->chain.begin() + idx);
}

static int remove_in_range_locked(Target* t, uintptr_t lo, uintptr_t hi) {
    int removed = 0;
    for (size_t i = t->chain.size(); i-- > 0;) {
        uintptr_t d = (uintptr_t)t->chain[i].detour;
        if (d >= lo && d < hi) {
            remove_link(t, i);
            removed++;
        }
    }
    return removed;
}

int remove_in_range(void* target, uintptr_t lo, uintptr_t hi) {
    std::lock_guard lock(g_lock);
    Target* t = find_target(target);
    return t ? remove_in_range_locked(t, lo, hi) : 0;
}

int remove_by_detour_range(uintptr_t lo, uintptr_t hi) {
    std::lock_guard lock(g_lock);
    int removed = 0;
    for (auto& t : g_targets)
        removed += remove_in_range_locked(t.get(), lo, hi);
    return removed;
}

}
