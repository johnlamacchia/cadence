#include "proxy_table.h"
#include "loader.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        if (!proxy_init()) {
            MessageBoxW(nullptr, L"Failed to load real winmm.dll", L"Cadence Loader", MB_ICONERROR);
            return FALSE;
        }
        loader::init();
    }
    return TRUE;
}
