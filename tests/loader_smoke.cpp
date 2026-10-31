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

    // Cadence's by-name exports (see loader.cpp). Exit codes 10+ say which one is wrong.
    auto version = reinterpret_cast<int(*)()>(GetProcAddress(loader, "cadence_loader_version"));
    auto claim = reinterpret_cast<void(*)(int)>(GetProcAddress(loader, "cadence_claim_input"));
    auto claimed = reinterpret_cast<int(*)()>(GetProcAddress(loader, "cadence_input_claimed"));
    if (!GetProcAddress(loader, "cadence_reload_mods")) return 10;
    if (!version || version() != 3) return 11;
    if (!claim || !claimed) return 12;
    if (claimed()) return 13;                    // nothing has claimed the keyboard yet
    claim(1); claim(1);
    if (!claimed()) return 14;                   // claims are counted: two claims...
    claim(0);
    if (!claimed()) return 15;                   // ...need two releases
    claim(0);
    if (claimed()) return 16;
    claim(0);                                    // releasing more than was claimed must not go negative
    claim(1);
    if (!claimed()) return 17;
    claim(0);
    if (claimed()) return 18;
    return 0;
}
