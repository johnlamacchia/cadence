// Loads the real Cadence loader (winmm.dll) from the folder this exe lives in, so
// loader.log can be inspected. Used by tests/run_loader_smoke.ps1 with an empty
// address file: nothing gets hooked, but loading, ordering, metadata, layout-hash
// checks and the "address not found, skip" path all run for real.
#include <windows.h>
#include <string>

int main() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    std::wstring dir(path);
    dir.resize(dir.find_last_of(L'\\'));
    HMODULE loader = LoadLibraryW((dir + L"\\winmm.dll").c_str());
    if (!loader) return 2;
    Sleep(1500);   // let the loader's threads start
    return 0;
}
