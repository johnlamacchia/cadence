// Optional helpers. The loader and sdk.h do not depend on this file, and a mod
// that does not include it pays nothing. Two things nearly every gameplay mod
// needs and everyone gets subtly wrong when they write it themselves.
#pragma once
#include <sdk.h>
#include <gd/GameManager.hpp>
#include <fstream>
#include <string>
#include <vector>

namespace cadence {

// The live PlayLayer, or nullptr when no level is loaded. Game thread only.
//
// Use this instead of remembering the pointer from a PlayLayer::init hook: the
// game clears GameManager::m_playLayer in ~PlayLayer, so this can never dangle,
// while a pointer you cached can (two of our own mods crashed that way).
inline PlayLayer* play_layer() {
    using shared_t = GameManager* (*)();
    static shared_t shared = nullptr;
    if (!shared) shared = (shared_t)addr(fn_sig<&GameManager::sharedState>);
    GameManager* gm = shared ? shared() : nullptr;
    return gm ? gm->m_playLayer : nullptr;
}

#ifdef _WIN32
// ---- Typing --------------------------------------------------------------------------------
//
// A mod that reads typed text (a search box) calls claim_input(true) while it does and claim_input(false) when
// it is done. While any claim is held, key_pressed() reports nothing, so mods that poll keys do not also react
// to every letter meant for the text box (typing "h" must not toggle the hitboxes). Claims are counted, and the
// loader clears them on a reload. Needs loader version 3; on an older loader both do nothing and false is returned.
inline bool input_claimed() {
    static int(*claimed)() = [] {
        HMODULE loader = GetModuleHandleW(L"winmm.dll");     // the loader is Cadence's winmm.dll proxy
        return loader ? reinterpret_cast<int(*)()>(GetProcAddress(loader, "cadence_input_claimed")) : nullptr;
    }();
    return claimed && claimed();
}

inline void claim_input(bool claim) {
    static void(*set)(int) = [] {
        HMODULE loader = GetModuleHandleW(L"winmm.dll");
        return loader ? reinterpret_cast<void(*)(int)>(GetProcAddress(loader, "cadence_claim_input")) : nullptr;
    }();
    if (set) set(claim ? 1 : 0);
}

// True once per physical key press, and only while Geometry Dash is the
// foreground window. Call it at most once per key per frame, from a game-thread
// hook (CCDirector::drawScene is a good one). No polling thread needed.
// Always false while a mod holds claim_input(true), see above.
inline bool key_pressed(int vk) {
    static bool was_down[256] = {};
    if (vk < 0 || vk > 255) return false;
    DWORD pid = 0;
    GetWindowThreadProcessId(GetForegroundWindow(), &pid);
    bool down = pid == GetCurrentProcessId() && (GetAsyncKeyState(vk) & 0x8000) != 0;
    bool pressed = down && !was_down[vk];
    was_down[vk] = down;                       // keep tracking while claimed, so a held key is not a new press afterwards
    return pressed && !input_claimed();
}
#endif

#ifdef _WIN32
// ---- Switching mods on and off --------------------------------------------------------------
//
// The loader skips every mod named in <game folder>/cadence_disabled.txt (one mod file name per line,
// e.g. "hitboxes.dll"; # starts a comment) and still lists it in api->get_mods with enabled = false.
// These helpers read and write that file and ask the loader to re-read it. A mod must not disable
// itself: that would leave nothing to switch it back on.
//
// Both loader features arrive in loader version 2. On an older loader, loader_version() is 1,
// reload_mods() returns false, and the file is ignored.

inline std::wstring game_folder() {
    wchar_t exe[MAX_PATH];
    DWORD n = GetModuleFileNameW(nullptr, exe, MAX_PATH);
    std::wstring path(exe, n);
    return path.substr(0, path.find_last_of(L"\\/") + 1);
}

// 1 for a loader with no cadence_loader_version export (1.1.x and older), else its version.
inline int loader_version() {
    HMODULE loader = GetModuleHandleW(L"winmm.dll");     // the loader is Cadence's winmm.dll proxy
    auto version = loader ? reinterpret_cast<int(*)()>(GetProcAddress(loader, "cadence_loader_version")) : nullptr;
    return version ? version() : 1;
}

// Same as pressing F6: the loader reloads every mod on the next frame. False if the loader is too old.
inline bool reload_mods() {
    HMODULE loader = GetModuleHandleW(L"winmm.dll");
    auto reload = loader ? reinterpret_cast<void(*)()>(GetProcAddress(loader, "cadence_reload_mods")) : nullptr;
    if (!reload) return false;
    reload();
    return true;
}

// Mod file names currently listed in cadence_disabled.txt (comments and blank lines skipped).
inline std::vector<std::string> disabled_mods() {
    std::vector<std::string> names;
    std::ifstream file(game_folder() + L"cadence_disabled.txt");
    for (std::string line; std::getline(file, line);) {
        size_t first = line.find_first_not_of(" \t");
        size_t last = line.find_last_not_of(" \t\r");
        if (first == std::string::npos || line[first] == '#') continue;
        names.push_back(line.substr(first, last - first + 1));
    }
    return names;
}

// Replaces cadence_disabled.txt with exactly these names. Takes effect at the next reload.
inline bool write_disabled_mods(const std::vector<std::string>& names) {
    std::ofstream file(game_folder() + L"cadence_disabled.txt", std::ios::trunc);
    if (!file) return false;
    file << "# Mods listed here are not run. Written by the Cadence menu; edit freely.\n";
    for (const std::string& name : names) file << name << '\n';
    return static_cast<bool>(file);
}
#endif
} // namespace cadence
