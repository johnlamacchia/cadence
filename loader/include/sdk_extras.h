// Optional helpers. The loader and sdk.h do not depend on this file, and a mod
// that does not include it pays nothing. Two things nearly every gameplay mod
// needs and everyone gets subtly wrong when they write it themselves.
#pragma once
#include <sdk.h>
#include <gd/GameManager.hpp>

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
// True once per physical key press, and only while Geometry Dash is the
// foreground window. Call it at most once per key per frame, from a game-thread
// hook (CCDirector::drawScene is a good one). No polling thread needed.
inline bool key_pressed(int vk) {
    static bool was_down[256] = {};
    if (vk < 0 || vk > 255) return false;
    DWORD pid = 0;
    GetWindowThreadProcessId(GetForegroundWindow(), &pid);
    bool down = pid == GetCurrentProcessId() && (GetAsyncKeyState(vk) & 0x8000) != 0;
    bool pressed = down && !was_down[vk];
    was_down[vk] = down;
    return pressed;
}
#endif

} // namespace cadence
