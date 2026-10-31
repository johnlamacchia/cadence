// Cadence menu: press Insert to list every installed mod, switch mods on and off, and read what each
// one does. Built with sdk_ui.h, so it looks like the game's own popups. The list scrolls with the mouse wheel,
// has page arrows (only when there is another page), and filters as you type after clicking Search.
//
// Switching a mod off writes its file name to cadence_disabled.txt (the loader skips those). Changes
// are collected in the list and applied with the Apply button, which saves the file, closes the menu
// and reloads every mod, so the game never changes under you while you are choosing.
#include <sdk_extras.h>
#include <sdk_ui.h>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace {

constexpr float kAskWidth = 480.0f;
constexpr float kAskHeight = 300.0f;
constexpr float kRowStep = 26.0f;

enum class View { List, Detail };

struct Row {
    std::string filename, name, author, version, description, key;
    bool running;
};

struct Menu {
    std::unique_ptr<cadence::ui::Popup> popup;
    View view = View::List;
    std::vector<Row> rows;
    std::set<std::string> disabled;         // lowercase file names the player wants switched off
    std::set<std::string> initial;          // what the file said when the menu opened
    int first = 0, visible = 1, shown = 0;  // first visible row of the (filtered) list, rows per page, rows in the list
    size_t detail = 0;                      // index into rows, not into the filtered list
    std::string query;                      // what the player typed into the search box (lowercase)
    bool typing = false;                    // true while the keyboard is ours: see cadence::claim_input
    bool rebuild = false;
    std::string note;                       // a line under the list (shown after Apply on an old loader)
} g;

std::string lowercase(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// This DLL's name as the player knows it: the loader runs a copy called "modmenu.3.dll".
std::string own_filename() {
    HMODULE self = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCWSTR>(&own_filename), &self);
    wchar_t path[MAX_PATH];
    DWORD n = GetModuleFileNameW(self, path, MAX_PATH);
    std::wstring name(path, n);
    name = name.substr(name.find_last_of(L"\\/") + 1);
    size_t dot = name.rfind(L'.');
    std::wstring stem = name.substr(0, dot);
    size_t gen = stem.rfind(L'.');
    if (gen != std::wstring::npos && stem.find_first_not_of(L"0123456789", gen + 1) == std::wstring::npos)
        stem.resize(gen);
    std::string out;
    for (wchar_t c : stem + L".dll") out += static_cast<char>(c);
    return lowercase(out);
}

void collect_rows() {
    g.rows.clear();
    int count = 0;
    const ModEntry* mods = cadence::get_mods(&count);
    for (int i = 0; mods && i < count; i++) {
        Row r;
        r.filename = mods[i].filename ? mods[i].filename : "";
        r.name = mods[i].name ? mods[i].name : r.filename;
        r.author = mods[i].author ? mods[i].author : "";
        r.version = mods[i].version ? mods[i].version : "";
        r.description = mods[i].description ? mods[i].description : "";
        r.key = mods[i].keybind ? mods[i].keybind : "";
        r.running = mods[i].enabled;
        g.rows.push_back(std::move(r));
    }
    std::sort(g.rows.begin(), g.rows.end(),
              [](const Row& a, const Row& b) { return lowercase(a.name) < lowercase(b.name); });
}

void request_rebuild() { g.rebuild = true; }

bool matches(const Row& r) {
    if (g.query.empty()) return true;
    return lowercase(r.name + " " + r.author + " " + r.description + " " + r.filename).find(g.query) != std::string::npos;
}

// While typing the keyboard belongs to the search box: other mods that poll keys through cadence::key_pressed
// ignore it (so typing "h" does not toggle the hitboxes), and the keys never reach the game (see hook_key).
void set_typing(bool on) {
    if (on == g.typing) return;
    g.typing = on;
    cadence::claim_input(on);
    request_rebuild();
}

void set_query(const std::string& q) {
    g.query = q;
    g.first = 0;
    request_rebuild();
}

void scroll_by(int rows) {
    int max_first = std::max(0, g.shown - g.visible);
    g.first = std::clamp(g.first + rows, 0, max_first);
    request_rebuild();
}

std::vector<std::string> wrap(const std::string& text, size_t width) {
    std::vector<std::string> lines;
    std::string line, word;
    auto flush_word = [&] {
        if (word.empty()) return;
        if (!line.empty() && line.size() + 1 + word.size() > width) { lines.push_back(line); line.clear(); }
        if (!line.empty()) line += ' ';
        line += word;
        word.clear();
    };
    for (char ch : text) {
        if (ch == ' ' || ch == '\n') flush_word();
        else word += ch;
    }
    flush_word();
    if (!line.empty()) lines.push_back(line);
    return lines;
}

// Regions relative to the window centre; the game places its own title and Close button.
struct Frame {
    float w, h;
    float header_y() const { return h / 2 - 60; }
    float body_top() const { return h / 2 - 90; }
    float body_bottom() const { return -h / 2 + 66; }
    float footer_y() const { return -h / 2 + 50; }
    float left() const { return -w / 2 + 40.0f; }
};

bool changed() { return g.disabled != g.initial; }

void apply() {
    std::vector<std::string> names(g.disabled.begin(), g.disabled.end());
    if (!cadence::write_disabled_mods(names)) {
        g.note = "Could not write cadence_disabled.txt";
        request_rebuild();
        return;
    }
    cadence::log("modmenu: saved %zu disabled mod(s)", names.size());
    if (g.popup) g.popup->close();
    if (!cadence::reload_mods())
        cadence::log("modmenu: this loader cannot reload by itself; restart the game to apply");
}

void populate_list(cadence::ui::Popup& p, const Frame& f) {
    char buf[160];
    int total = static_cast<int>(g.rows.size());
    int off = 0;
    for (const Row& r : g.rows) off += g.disabled.count(lowercase(r.filename)) ? 1 : 0;

    // Header line: the summary on the left, the search box on the right.
    snprintf(buf, sizeof(buf), "%d mods, %d switched off   |   GD %s   |   loader %d", total, off, cadence::gd_version(),
             cadence::loader_version());
    p.label(buf, f.left(), f.header_y(), 0.28f, "chatFont.fnt", true, 190.0f);
    p.button(g.typing ? "Done" : "Search", f.w / 2 - 56.0f, f.header_y(), [] { set_typing(!g.typing); }, 0.32f, 20.0f,
             g.typing ? "GJ_button_02.png" : "GJ_button_01.png", 62.0f);
    if (g.typing) snprintf(buf, sizeof(buf), "%s_", g.query.c_str());
    else if (!g.query.empty()) snprintf(buf, sizeof(buf), "%s", g.query.c_str());
    else snprintf(buf, sizeof(buf), "(click Search to filter)");
    p.label(buf, f.w / 2 - 222.0f, f.header_y(), 0.32f, g.typing ? "bigFont.fnt" : "chatFont.fnt", true, 128.0f);

    // The filtered list.
    std::vector<int> view;
    for (int i = 0; i < total; i++)
        if (matches(g.rows[i])) view.push_back(i);
    g.shown = static_cast<int>(view.size());
    g.visible = std::max(1, static_cast<int>((f.body_top() - f.body_bottom()) / kRowStep) + 1);
    g.first = std::clamp(g.first, 0, std::max(0, g.shown - g.visible));

    if (view.empty()) {
        snprintf(buf, sizeof(buf), g.query.empty() ? "No mods installed." : "No mod matches \"%s\".", g.query.c_str());
        p.label(buf, 0.0f, f.body_top() - 20.0f, 0.4f, "bigFont.fnt");
    }

    const std::string self = own_filename();
    const float name_x = f.left() + 18.0f;
    const float meta_x = name_x + 215.0f;
    const float info_x = f.w / 2 - 56.0f;
    float y = f.body_top();
    int last = std::min(g.shown, g.first + g.visible);
    for (int v = g.first; v < last; v++, y -= kRowStep) {
        const int i = view[v];
        const Row& r = g.rows[i];
        const std::string key = lowercase(r.filename);
        if (key == self) {
            p.label(r.name.c_str(), name_x + 22.0f, y, 0.36f, "goldFont.fnt", true, 170.0f);   // never switch the menu off
        } else {
            p.checkbox(r.name.c_str(), name_x, y, !g.disabled.count(key),
                       [key](bool on) { if (on) g.disabled.erase(key); else g.disabled.insert(key); request_rebuild(); },
                       0.36f, 0.6f, 170.0f);
        }
        snprintf(buf, sizeof(buf), "%s%s%s", r.version.empty() ? "" : ("v" + r.version).c_str(),
                 (r.version.empty() || r.author.empty()) ? "" : "  by ", r.author.c_str());
        p.label(buf, meta_x, y, 0.3f, "chatFont.fnt", true, 120.0f);
        p.icon_button("GJ_infoIcon_001.png", info_x, y, [i] { set_typing(false); g.view = View::Detail; g.detail = i; request_rebuild(); }, 0.5f);
    }

    // Page arrows, only where there is another page to go to.
    const float mid = (f.body_top() + f.body_bottom()) / 2.0f;
    const int pages = std::max(1, (g.shown + g.visible - 1) / g.visible);
    const int page = std::min(pages, g.first / g.visible + 1);
    if (g.first > 0)
        p.icon_button("GJ_arrow_03_001.png", -f.w / 2 + 26.0f, mid, [] { g.first = std::max(0, g.first - g.visible); request_rebuild(); }, 0.55f, true);
    if (g.first + g.visible < g.shown)
        p.icon_button("GJ_arrow_03_001.png", f.w / 2 - 26.0f, mid, [] { g.first += g.visible; request_rebuild(); }, 0.55f);
    if (pages > 1) {
        snprintf(buf, sizeof(buf), "Page %d / %d", page, pages);
        p.label(buf, f.left(), f.footer_y(), 0.3f, "chatFont.fnt", true, 80.0f);
    }

    if (changed()) {
        p.button("Apply and reload", 0.0f, f.footer_y(), apply, 0.5f, 26.0f, "GJ_button_02.png");
    } else if (!g.note.empty()) {
        p.label(g.note.c_str(), 0.0f, f.footer_y(), 0.32f, "chatFont.fnt");
    } else {
        p.label("Tick a mod to run it, untick to switch it off, then Apply.", 0.0f, f.footer_y(), 0.3f, "chatFont.fnt");
    }
}
void populate_detail(cadence::ui::Popup& p, const Frame& f) {
    if (g.detail >= g.rows.size()) { g.view = View::List; return; }
    const Row& r = g.rows[g.detail];
    char buf[200];

    p.label(r.name.c_str(), 0.0f, f.h / 2 - 66.0f, 0.55f, "goldFont.fnt", false, f.w - 80.0f);
    snprintf(buf, sizeof(buf), "%s%s%s%s", r.version.empty() ? "" : "version ", r.version.c_str(),
             r.author.empty() ? "" : "     by ", r.author.c_str());
    p.label(buf, 0.0f, f.h / 2 - 90.0f, 0.34f, "chatFont.fnt");

    float y = f.h / 2 - 116.0f;
    const std::string desc = r.description.empty() ? "(no description)" : r.description;
    for (const std::string& line : wrap(desc, static_cast<size_t>((f.w - 70.0f) / 5.5f))) {
        p.label(line.c_str(), 0.0f, y, 0.5f, "chatFont.fnt");
        y -= 15.0f;
    }
    y -= 10.0f;
    if (!r.key.empty()) {
        snprintf(buf, sizeof(buf), "Key: %s", r.key.c_str());
        p.label(buf, 0.0f, y, 0.4f, "bigFont.fnt");
        y -= 20.0f;
    }
    snprintf(buf, sizeof(buf), "%s   (%s)", r.filename.c_str(), r.running ? "running" : "switched off");
    p.label(buf, 0.0f, y, 0.3f, "chatFont.fnt");

    p.button("Back", 0.0f, f.footer_y(), [] { g.view = View::List; request_rebuild(); }, 0.5f, 26.0f);
}

void populate() {
    if (!g.popup) return;
    g.popup->clear();
    Frame f{g.popup->width(), g.popup->height()};
    if (g.view == View::List) populate_list(*g.popup, f);
    else populate_detail(*g.popup, f);
}

void open_menu() {
    collect_rows();
    g.disabled.clear();
    for (const std::string& name : cadence::disabled_mods()) g.disabled.insert(lowercase(name));
    g.initial = g.disabled;
    g.view = View::List;
    g.first = 0;
    g.query.clear();
    set_typing(false);
    g.note.clear();
    g.rebuild = false;
    g.popup = std::make_unique<cadence::ui::Popup>("Cadence", kAskWidth, kAskHeight, true);
    populate();
    cadence::log("modmenu: opened, %zu mods listed", g.rows.size());
    g.popup->show();
}

void(__fastcall* orig_drawScene)(void*) = nullptr;
using dispatch_scroll_t = bool(__fastcall*)(void* self, float y, float x);
dispatch_scroll_t orig_scroll = nullptr;
using dispatch_key_t = bool(__fastcall*)(void* self, int key, bool down, bool repeat, double timestamp);
dispatch_key_t orig_key = nullptr;

void __fastcall hook_drawScene(void* director) {
    if (g.typing && !(g.popup && g.popup->is_open())) set_typing(false);      // closed with the Close button mid-search
    if (cadence::key_pressed(VK_INSERT)) {                                      // (false while typing: Done / Enter first)
        if (g.popup && g.popup->is_open()) g.popup->close();
        else open_menu();
    }
    if (g.rebuild) {
        g.rebuild = false;
        if (g.popup && g.popup->is_open()) populate();
    }
    orig_drawScene(director);
}

// While the player is typing in the search box every key belongs to it: letters and digits are added, Backspace
// removes, Enter ends the search, Escape clears it and ends it. None of them reach the game (so Escape does not
// also close the popup).
bool __fastcall hook_key(void* self, int key, bool down, bool repeat, double timestamp) {
    if (!g.typing || !(g.popup && g.popup->is_open()) || g.view != View::List)
        return orig_key(self, key, down, repeat, timestamp);
    if (down) {
        if (key == VK_RETURN) {
            set_typing(false);
        } else if (key == VK_ESCAPE) {
            set_typing(false);
            set_query("");
        } else if (key == VK_BACK) {
            if (!g.query.empty()) { std::string q = g.query; q.pop_back(); set_query(q); }
        } else if (g.query.size() < 24) {
            char c = 0;
            if ((key >= 'A' && key <= 'Z')) c = static_cast<char>(key - 'A' + 'a');
            else if ((key >= '0' && key <= '9') || key == ' ') c = static_cast<char>(key);
            else if (key == VK_OEM_MINUS) c = '-';
            if (c) set_query(g.query + c);
        }
    }
    return false;
}

// While the menu is open the mouse wheel scrolls its list and is not passed on to the game.
bool __fastcall hook_scroll(void* self, float y, float x) {
    if (g.popup && g.popup->is_open() && g.view == View::List && y != 0.0f) {
        scroll_by(y > 0.0f ? -1 : +1);
        return true;
    }
    return orig_scroll(self, y, x);
}

} // namespace

extern "C" __declspec(dllexport) void mod_unload() {
    set_typing(false);                                         // give the keyboard back
    if (g.popup && g.popup->is_open()) g.popup->close();      // no popup left pointing into a retired mod
    g.popup.reset();
}

extern "C" __declspec(dllexport) void mod_main(const LoaderAPI* api) {
    cadence::set_api(api);
    void* draw_scene = cadence::cocos("?drawScene@CCDirector@cocos2d@@QEAAXXZ");
    if (!draw_scene || !cadence::hook(draw_scene, reinterpret_cast<void*>(hook_drawScene),
                                      reinterpret_cast<void**>(&orig_drawScene))) {
        cadence::log("modmenu: could not hook drawScene, the menu will not open");
        return;
    }
    if (void* scroll = cadence::cocos("?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QEAA_NMM@Z"))
        cadence::hook(scroll, reinterpret_cast<void*>(hook_scroll), reinterpret_cast<void**>(&orig_scroll));
    if (void* key = cadence::cocos("?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QEAA_NW4enumKeyCodes@2@_N1N@Z"))
        cadence::hook(key, reinterpret_cast<void*>(hook_key), reinterpret_cast<void**>(&orig_key));
    cadence::log("modmenu: ready, press Insert (loader version %d)", cadence::loader_version());
}
