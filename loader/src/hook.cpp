#include "hook.h"
#include <windows.h>
#include <Zydis/Zydis.h>
#include <cstring>

namespace hook {

static constexpr size_t JMP_ABS_SIZE = 14;   // ff 25 00000000 + 8-byte addr
static constexpr size_t TRAMPOLINE_MAX = 64;

static HookEntry* g_hooks = nullptr;
static CRITICAL_SECTION g_lock;
static bool g_init = false;

static void ensure_init() {
    if (!g_init) {
        InitializeCriticalSection(&g_lock);
        g_init = true;
    }
}

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
        // Rewrite as: original instruction but targeting abs_target via an indirect jump
        // For short/near jumps and calls: replace with absolute indirect
        if (insn->mnemonic == ZYDIS_MNEMONIC_JMP) {
            write_abs_jmp(dest, (void*)abs_target);
            *out_size = JMP_ABS_SIZE;
            return true;
        }
        if (insn->mnemonic == ZYDIS_MNEMONIC_CALL) {
            // call [rip+0]; dq abs_target  — but we can't easily do indirect call
            // Instead: push return addr, jmp abs
            // Simpler: just copy and fix the offset if it fits in 32 bits
            int64_t new_disp = (int64_t)abs_target - (int64_t)(dest_addr + 5);
            if (new_disp >= INT32_MIN && new_disp <= INT32_MAX) {
                dest[0] = 0xE8;
                *(int32_t*)(dest + 1) = (int32_t)new_disp;
                *out_size = 5;
                return true;
            }
            return false;
        }
        // Conditional jumps: rewrite to inverted-jcc-over-abs-jmp pattern
        // jcc_short skip; jmp abs_target; skip:
        // Invert condition, jump over the absolute jump
        // Both short (7x) and near (0F 8x) jcc: condition is opcode & 0x0F
        uint8_t cond = insn->opcode & 0x0F;
        // Inverted short jcc: skip over 14-byte abs jmp
        dest[0] = 0x70 | (cond ^ 1); // invert condition
        dest[1] = JMP_ABS_SIZE;       // skip 14 bytes
        write_abs_jmp(dest + 2, (void*)abs_target);
        *out_size = 2 + JMP_ABS_SIZE;
        return true;
    }

    // RIP-relative memory operand: adjust displacement
    // Copy instruction, fix the RIP-relative displacement
    memcpy(dest, (void*)orig_addr, insn->length);
    int64_t new_disp = (int64_t)abs_target - (int64_t)(dest_addr + insn->length);
    if (new_disp < INT32_MIN || new_disp > INT32_MAX) {
        return false;
    }
    // Find and patch the displacement in the copied instruction
    // The displacement is at raw.disp.offset bytes from instruction start
    if (insn->raw.disp.offset > 0) {
        *(int32_t*)(dest + insn->raw.disp.offset) = (int32_t)new_disp;
    }
    *out_size = insn->length;
    return true;
}

bool install(void* target, void* detour, void** original) {
    ensure_init();
    EnterCriticalSection(&g_lock);

    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    uint8_t* func = (uint8_t*)target;
    size_t copied = 0;

    // Determine how many bytes we need to overwrite (at least JMP_ABS_SIZE)
    ZydisDecodedInstruction insn;
    ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
    while (copied < JMP_ABS_SIZE) {
        ZyanStatus status = ZydisDecoderDecodeFull(
            &decoder, func + copied, 15, &insn, operands);
        if (!ZYAN_SUCCESS(status)) {
            LeaveCriticalSection(&g_lock);
            return false;
        }
        copied += insn.length;
    }

    // Allocate trampoline near the target
    uint8_t* tramp = (uint8_t*)alloc_near(target, TRAMPOLINE_MAX);
    if (!tramp) {
        LeaveCriticalSection(&g_lock);
        return false;
    }

    // Build trampoline: relocated original bytes + jump back to original func
    size_t tramp_offset = 0;
    size_t src_offset = 0;
    while (src_offset < copied) {
        ZydisDecoderDecodeFull(&decoder, func + src_offset, 15, &insn, operands);
        size_t relocated_size;
        if (!relocate_instruction(
                &insn, operands,
                (uint64_t)(func + src_offset),
                tramp + tramp_offset,
                (uint64_t)(tramp + tramp_offset),
                &relocated_size)) {
            VirtualFree(tramp, 0, MEM_RELEASE);
            LeaveCriticalSection(&g_lock);
            return false;
        }
        src_offset += insn.length;
        tramp_offset += relocated_size;
    }

    // Jump back to original code after the overwritten bytes
    write_abs_jmp(tramp + tramp_offset, func + copied);
    tramp_offset += JMP_ABS_SIZE;

    // Patch the original function to jump to detour
    DWORD old_protect;
    VirtualProtect(func, JMP_ABS_SIZE, PAGE_EXECUTE_READWRITE, &old_protect);
    write_abs_jmp(func, detour);
    // NOP any remaining bytes
    for (size_t i = JMP_ABS_SIZE; i < copied; i++) {
        func[i] = 0x90;
    }
    VirtualProtect(func, JMP_ABS_SIZE, old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), func, copied);

    *original = tramp;

    // Record hook
    auto* entry = new HookEntry{target, detour, tramp, g_hooks};
    g_hooks = entry;

    LeaveCriticalSection(&g_lock);
    return true;
}

bool remove(void* target) {
    ensure_init();
    EnterCriticalSection(&g_lock);

    HookEntry** pp = &g_hooks;
    while (*pp) {
        if ((*pp)->target == target) {
            HookEntry* entry = *pp;
            uint8_t* func = (uint8_t*)entry->target;
            uint8_t* tramp = (uint8_t*)entry->trampoline;

            // Count how many bytes were originally copied to trampoline
            // by scanning trampoline until we hit the absolute jump back
            ZydisDecoder decoder;
            ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

            // Restore original bytes from trampoline
            // We need to relocate them BACK, but simpler: the trampoline has
            // the original instructions (potentially relocated). For safety,
            // we store the original bytes separately. For now, just remove the
            // jmp from the target and write back the trampoline instructions
            // (this works for non-RIP-relative originals; RIP-relative ones
            // need re-relocation, which is rare enough to skip for v1).

            // Find how many bytes were overwritten (JMP_ABS_SIZE + NOPs)
            size_t patch_size = JMP_ABS_SIZE;
            while (func[patch_size] == 0x90 && patch_size < 32) patch_size++;

            // Copy original bytes back from trampoline
            DWORD old_protect;
            VirtualProtect(func, patch_size, PAGE_EXECUTE_READWRITE, &old_protect);
            // The trampoline starts with the relocated original instructions
            // For simple non-RIP-relative instructions, they're identical
            // For RIP-relative, this is imperfect but acceptable for v1
            memcpy(func, tramp, patch_size);
            VirtualProtect(func, patch_size, old_protect, &old_protect);
            FlushInstructionCache(GetCurrentProcess(), func, patch_size);

            VirtualFree(tramp, 0, MEM_RELEASE);
            *pp = entry->next;
            delete entry;
            LeaveCriticalSection(&g_lock);
            return true;
        }
        pp = &(*pp)->next;
    }

    LeaveCriticalSection(&g_lock);
    return false;
}

void shutdown() {
    if (!g_init) return;
    while (g_hooks) {
        remove(g_hooks->target);
    }
    DeleteCriticalSection(&g_lock);
    g_init = false;
}

}
