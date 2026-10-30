#include "crash.h"
#include <windows.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <cstdio>
#include <ctime>
#include <filesystem>

#pragma comment(lib, "dbghelp.lib")

namespace crash {

static FILE* open_crash_log(const wchar_t* dir, wchar_t* dmp_path_out, size_t dmp_path_size) {
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y%m%d_%H%M%S", &t);

    wchar_t log_path[MAX_PATH];
    swprintf_s(log_path, L"%s\\crash_%S.log", dir, ts);
    swprintf_s(dmp_path_out, dmp_path_size, L"%s\\crash_%S.dmp", dir, ts);

    return _wfopen(log_path, L"w");
}

static const char* exception_name(DWORD code) {
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION:      return "ACCESS_VIOLATION";
        case EXCEPTION_STACK_OVERFLOW:        return "STACK_OVERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "INT_DIVIDE_BY_ZERO";
        case EXCEPTION_ILLEGAL_INSTRUCTION:   return "ILLEGAL_INSTRUCTION";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:    return "FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_PRIV_INSTRUCTION:      return "PRIV_INSTRUCTION";
        case EXCEPTION_IN_PAGE_ERROR:         return "IN_PAGE_ERROR";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "NONCONTINUABLE";
        default:                              return "UNKNOWN";
    }
}

static bool is_fatal(DWORD code) {
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION:
        case EXCEPTION_STACK_OVERFLOW:
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
        case EXCEPTION_ILLEGAL_INSTRUCTION:
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        case EXCEPTION_PRIV_INSTRUCTION:
        case EXCEPTION_IN_PAGE_ERROR:
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return true;
        default:
            return false;
    }
}

struct ModuleInfo {
    wchar_t name[MAX_PATH];
    uintptr_t base;
    uintptr_t end;
};

static int enum_modules(ModuleInfo* out, int max_count) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (snap == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32W me;
    me.dwSize = sizeof(me);
    int count = 0;
    if (Module32FirstW(snap, &me)) {
        do {
            if (count < max_count) {
                wcscpy_s(out[count].name, me.szModule);
                out[count].base = (uintptr_t)me.modBaseAddr;
                out[count].end = out[count].base + me.modBaseSize;
                count++;
            }
        } while (Module32NextW(snap, &me));
    }
    CloseHandle(snap);
    return count;
}

static const wchar_t* find_module(ModuleInfo* mods, int count, uintptr_t addr) {
    for (int i = 0; i < count; i++) {
        if (addr >= mods[i].base && addr < mods[i].end)
            return mods[i].name;
    }
    return L"<unknown>";
}

static LONG WINAPI handler(EXCEPTION_POINTERS* ep) {
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    if (!is_fatal(code)) return EXCEPTION_CONTINUE_SEARCH;

    wchar_t exe_dir[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_dir, MAX_PATH);
    std::filesystem::path p(exe_dir);
    auto dir = p.parent_path().wstring();

    wchar_t dmp_path[MAX_PATH];
    FILE* f = open_crash_log(dir.c_str(), dmp_path, MAX_PATH);
    if (!f) return EXCEPTION_CONTINUE_SEARCH;

    uintptr_t crash_addr = (uintptr_t)ep->ExceptionRecord->ExceptionAddress;

    ModuleInfo mods[256];
    int mod_count = enum_modules(mods, 256);
    const wchar_t* crash_mod = find_module(mods, mod_count, crash_addr);

    fprintf(f, "=== Cadence Crash Report ===\n\n");
    fprintf(f, "Exception: 0x%08lX (%s)\n", code, exception_name(code));
    fprintf(f, "Address:   0x%llX\n", (unsigned long long)crash_addr);
    fprintf(f, "Module:    %ls\n\n", crash_mod);

    if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2) {
        const char* op = ep->ExceptionRecord->ExceptionInformation[0] == 0 ? "read" : "write";
        fprintf(f, "Attempted %s of address 0x%llX\n\n", op,
            (unsigned long long)ep->ExceptionRecord->ExceptionInformation[1]);
    }

    auto* ctx = ep->ContextRecord;
    fprintf(f, "Registers:\n");
    fprintf(f, "  RAX = 0x%016llX  RBX = 0x%016llX\n", ctx->Rax, ctx->Rbx);
    fprintf(f, "  RCX = 0x%016llX  RDX = 0x%016llX\n", ctx->Rcx, ctx->Rdx);
    fprintf(f, "  RSI = 0x%016llX  RDI = 0x%016llX\n", ctx->Rsi, ctx->Rdi);
    fprintf(f, "  RSP = 0x%016llX  RBP = 0x%016llX\n", ctx->Rsp, ctx->Rbp);
    fprintf(f, "  R8  = 0x%016llX  R9  = 0x%016llX\n", ctx->R8, ctx->R9);
    fprintf(f, "  R10 = 0x%016llX  R11 = 0x%016llX\n", ctx->R10, ctx->R11);
    fprintf(f, "  R12 = 0x%016llX  R13 = 0x%016llX\n", ctx->R12, ctx->R13);
    fprintf(f, "  R14 = 0x%016llX  R15 = 0x%016llX\n", ctx->R14, ctx->R15);
    fprintf(f, "  RIP = 0x%016llX  RFLAGS = 0x%08lX\n\n", ctx->Rip, ctx->EFlags);

    fprintf(f, "Stack trace:\n");
    void* frames[64];
    USHORT frame_count = CaptureStackBackTrace(0, 64, frames, nullptr);
    for (USHORT i = 0; i < frame_count; i++) {
        uintptr_t addr = (uintptr_t)frames[i];
        const wchar_t* mod = find_module(mods, mod_count, addr);
        uintptr_t mod_base = 0;
        for (int j = 0; j < mod_count; j++) {
            if (addr >= mods[j].base && addr < mods[j].end) {
                mod_base = mods[j].base;
                break;
            }
        }
        fprintf(f, "  [%2d] 0x%llX  %ls+0x%llX\n", i,
            (unsigned long long)addr, mod,
            (unsigned long long)(addr - mod_base));
    }

    fprintf(f, "\nLoaded modules:\n");
    for (int i = 0; i < mod_count; i++) {
        fprintf(f, "  0x%llX - 0x%llX  %ls\n",
            (unsigned long long)mods[i].base,
            (unsigned long long)mods[i].end,
            mods[i].name);
    }

    fclose(f);

    HANDLE dmp_file = CreateFileW(dmp_path, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (dmp_file != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION mei;
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = ep;
        mei.ClientPointers = FALSE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
            dmp_file, MiniDumpNormal, &mei, nullptr, nullptr);
        CloseHandle(dmp_file);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

void init() {
    AddVectoredExceptionHandler(1, handler);
}

}
