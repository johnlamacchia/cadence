// Optional: GD's own UI from a mod. Popups, buttons and checkboxes that look and
// behave like the game's, built from the game's own classes (FLAlertLayer,
// ButtonSprite, CCMenuItemSpriteExtra, CCMenuItemToggler). Like sdk_extras.h,
// nothing else depends on this file.
//
//     #include <sdk_ui.h>
//
//     cadence::ui::alert("Hello", "This is a GD popup.");
//
//     cadence::ui::confirm("Restart?", "Start over from 0%?", "No", "Yes",
//                          [](bool yes) { if (yes) restart(); });
//
//     cadence::ui::Popup("My mod", 300, 180)
//         .label("Options", 0, 55)
//         .checkbox("Show hitboxes", -100, 20, g_hitboxes, [](bool on) { g_hitboxes = on; })
//         .button("Reset", 0, -50, [] { reset(); })
//         .show();
//
// Callbacks run on the game thread. Coordinates are relative to the centre of the
// screen (GD's design height is 320, so y runs roughly -160..160).
//
// How it works without subclassing any cocos class: a button needs a target object
// and a member-function pointer. We use one retained plain CCNode as the target and
// a free function as the "member function"; cocos calls it with the menu item as
// `sender`, and that pointer looks up your callback. Popups report their button
// through a hand-built FLAlertLayerProtocol (a pointer to a one-entry vtable).
#pragma once
#include <sdk_extras.h>
#include <gd/ButtonSprite.hpp>
#include <gd/CCMenuItemSpriteExtra.hpp>
#include <gd/CCMenuItemToggler.hpp>
#include <gd/cocos2d/CCMenu.hpp>
#include <gd/FLAlertLayer.hpp>
#include <gd/ScrollingLayer.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace cadence::ui {

namespace detail {

// The few cocos2d exports used to assemble nodes. Resolved on first use.
struct Cocos {
    void* (*label)(const char* text, const char* font) = nullptr;
    void* (*frame_sprite)(const char* frame_name) = nullptr;
    void* (*node)() = nullptr;
    void(__fastcall* retain)(void* object) = nullptr;
    void(__fastcall* add_child)(void* parent, void* child, int z) = nullptr;
    void(__fastcall* set_position)(void* node, float x, float y) = nullptr;
    void(__fastcall* set_scale)(void* node, float scale) = nullptr;
    void(__fastcall* set_anchor)(void* node, const cocos2d::CCPoint* anchor) = nullptr;
    void(__fastcall* remove_from_parent)(void* node) = nullptr;
    void(__fastcall* set_flip_x)(void* sprite, bool flip) = nullptr;       // optional: not needed for ok()

    bool ok() const {
        return label && frame_sprite && node && retain && add_child && set_position && set_scale && set_anchor &&
               remove_from_parent;
    }
};

template <class T>
T lookup(const char* mangled) { return reinterpret_cast<T>(cadence::cocos(mangled)); }

inline const Cocos& api() {
    static const Cocos c = [] {
        Cocos x;
        x.label = lookup<decltype(x.label)>("?create@CCLabelBMFont@cocos2d@@SAPEAV12@PEBD0@Z");
        x.frame_sprite = lookup<decltype(x.frame_sprite)>("?createWithSpriteFrameName@CCSprite@cocos2d@@SAPEAV12@PEBD@Z");
        x.node = lookup<decltype(x.node)>("?create@CCNode@cocos2d@@SAPEAV12@XZ");
        x.retain = lookup<decltype(x.retain)>("?retain@CCObject@cocos2d@@QEAAXXZ");
        x.add_child = lookup<decltype(x.add_child)>("?addChild@CCNode@cocos2d@@UEAAXPEAV12@H@Z");
        x.set_position = lookup<decltype(x.set_position)>("?setPosition@CCNode@cocos2d@@UEAAXMM@Z");
        x.set_scale = lookup<decltype(x.set_scale)>("?setScale@CCNode@cocos2d@@UEAAXM@Z");
        x.set_anchor = lookup<decltype(x.set_anchor)>("?setAnchorPoint@CCNode@cocos2d@@UEAAXAEBVCCPoint@2@@Z");
        x.remove_from_parent = lookup<decltype(x.remove_from_parent)>("?removeFromParent@CCNode@cocos2d@@UEAAXXZ");
        x.set_flip_x = lookup<decltype(x.set_flip_x)>("?setFlipX@CCSprite@cocos2d@@QEAAX_N@Z");
        if (!x.ok()) cadence::log("sdk_ui: a cocos2d export is missing, ui:: calls will do nothing");
        return x;
    }();
    return c;
}

// ---- Button callbacks ---------------------------------------------------------------

using Handler = std::function<void(void* sender)>;

inline std::unordered_map<void*, Handler>& handlers() {
    static std::unordered_map<void*, Handler> map;
    return map;
}

// Called by cocos as `(target->*selector)(sender)`; `target` is unused.
inline void __fastcall menu_trampoline(void*, void* sender) {
    auto it = handlers().find(sender);
    if (it == handlers().end()) return;
    Handler handler = it->second;        // copy: the handler may register more buttons
    handler(sender);
}

inline void* target_node() {
    static void* node = [] {
        void* n = api().node ? api().node() : nullptr;
        if (n) api().retain(n);          // never released: it lives as long as the game
        return n;
    }();
    return node;
}

inline cocos2d::SEL_MenuHandler selector() {
    cocos2d::SEL_MenuHandler s;
    s.fn = reinterpret_cast<void*>(&menu_trampoline);
    return s;
}

// A GD button (shrinks when pressed, like the game's) that runs `handler`.
inline CCMenuItemSpriteExtra* make_item(void* normal_sprite, Handler handler) {
    auto create = cadence::fn<static_cast<CCMenuItemSpriteExtra*(*)(
        cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler)>(
        &CCMenuItemSpriteExtra::create)>();
    if (!create || !normal_sprite || !target_node()) return nullptr;
    CCMenuItemSpriteExtra* item = create(static_cast<cocos2d::CCNode*>(normal_sprite), nullptr,
                                         static_cast<cocos2d::CCObject*>(target_node()), selector());
    if (item) handlers()[item] = std::move(handler);
    return item;
}

// ---- Popup result delegate ------------------------------------------------------------
//
// FLAlertLayerProtocol has one virtual function, so a delegate is "a pointer to a
// table whose first entry is FLAlert_Clicked". Building that by hand avoids
// inheriting a class whose vtable the headers can only declare.

struct Delegate {
    void* const* vptr;
    std::function<void(bool)> on_result;
    bool done = false;
};

inline std::vector<Delegate*>& live_delegates() {
    static std::vector<Delegate*> list;
    return list;
}

inline void __fastcall delegate_clicked(Delegate* self, FLAlertLayer*, bool second_button) {
    self->done = true;
    if (self->on_result) self->on_result(second_button);
}

inline void* const delegate_vtable[] = {reinterpret_cast<void*>(&delegate_clicked)};

// Delegates whose popup has reported its button are freed on the next call, once the
// popup that used them is certainly gone.
inline Delegate* new_delegate(std::function<void(bool)> on_result) {
    auto& list = live_delegates();
    for (size_t i = list.size(); i-- > 0;)
        if (list[i]->done) { delete list[i]; list.erase(list.begin() + i); }
    auto* d = new Delegate{delegate_vtable, std::move(on_result)};
    list.push_back(d);
    return d;
}

inline FLAlertLayer* create_alert(Delegate* delegate, const char* title, const std::string& text,
                                  const char* first, const char* second, float width, float height, bool scroll = false) {
    auto create = cadence::fn<static_cast<FLAlertLayer*(*)(
        FLAlertLayerProtocol*, char const*, std::string, char const*, char const*, float, bool, float, float)>(
        &FLAlertLayer::create)>();
    if (!create) return nullptr;
    return create(reinterpret_cast<FLAlertLayerProtocol*>(delegate), title, text, first, second, width,
                  scroll, height, 1.0f);
}

inline void show_layer(FLAlertLayer* layer) {
    if (auto show = cadence::fn<&FLAlertLayer::show>()) show(layer);
}

} // namespace detail

// ---- Simple popups ----------------------------------------------------------------------

// One-button message.
inline bool alert(const char* title, const std::string& text, const char* button = "OK") {
    FLAlertLayer* layer = detail::create_alert(nullptr, title, text, button, nullptr, 300.0f, 140.0f);
    if (!layer) return false;
    detail::show_layer(layer);
    return true;
}

// Two buttons; `on_result(true)` if the second one was pressed.
inline bool confirm(const char* title, const std::string& text, const char* no, const char* yes,
                    std::function<void(bool)> on_result) {
    detail::Delegate* d = detail::new_delegate(std::move(on_result));
    FLAlertLayer* layer = detail::create_alert(d, title, text, no, yes, 300.0f, 140.0f);
    if (!layer) { d->done = true; return false; }
    detail::show_layer(layer);
    return true;
}

// ---- Custom popups ------------------------------------------------------------------------

// A GD popup you add your own labels, buttons and checkboxes to. It always has a
// Close button. Build it, then call show(). Items land on the popup's button menu,
// so positions are relative to the centre of the screen.
//
// A popup can be rebuilt in place: clear() removes everything you added, then add
// again. Do that from your per-frame code, not from inside one of the popup's own
// button callbacks (a button must not delete itself while it is handling its click);
// set a flag in the callback and rebuild on the next frame.
//
// Popup is a cheap handle: copies share one popup.
class Popup {
public:
    // By default the game sizes the window to fit its (empty) text, so `height` is ignored and the
    // window is small. Pass `tall = true` to make `height` the real window height instead (the game
    // then lays the window out for a scrolling text area). Either way, read width()/height() back.
    Popup(const char* title, float width = 300.0f, float height = 180.0f, bool tall = false)
        : s_(std::make_shared<State>()) {
        std::shared_ptr<State> state = s_;
        detail::Delegate* d = detail::new_delegate([state](bool) { state->closed = true; });
        s_->layer = detail::create_alert(d, title, " ", "Close", nullptr, width, height, tall);
        if (!s_->layer) { d->done = true; s_->closed = true; return; }
        s_->width = s_->layer->m_width;
        s_->height = s_->layer->m_height;

        // Tall mode is only used for the window height; the empty scrolling text area it comes
        // with draws a shaded rectangle over everything we add, so hide it.
        if (tall && s_->layer->m_scrollingLayer) s_->layer->m_scrollingLayer->m_bVisible = false;

        // Where the button menu's origin is varies with how the game laid the window out (in the
        // tall mode it sits near the bottom, not at the centre). Measure it once and shift every
        // item by the difference, so coordinates really are relative to the window centre.
        if (s_->layer->m_buttonMenu) {
            s_->shift_x = s_->layer->m_obContentSize.width / 2 - s_->layer->m_buttonMenu->m_obPosition.x;
            s_->shift_y = s_->layer->m_obContentSize.height / 2 - s_->layer->m_buttonMenu->m_obPosition.y;
        }
        cadence::log("sdk_ui: popup \"%s\" is %g x %g (asked for %g x %g), button menu origin is off centre by (%g, %g)",
                     title, s_->width, s_->height, width, height, s_->shift_x, s_->shift_y);
    }

    // The window size the game actually gave us. Lay out from these, not from what you asked for.
    float width() const { return s_->width; }
    float height() const { return s_->height; }

    // True from show() until the player (or close()) dismisses it.
    bool is_open() const { return s_->shown && !s_->closed; }

    // Centred on (x, y), or with its left edge at x if `left_align`.
    // If `max_width` > 0 the scale is reduced until the text is at most that wide.
    Popup& label(const char* text, float x, float y, float scale = 0.5f, const char* font = "bigFont.fnt",
                 bool left_align = false, float max_width = 0.0f) {
        if (!usable()) return *this;
        if (void* l = detail::api().label(text, font)) {
            if (max_width > 0.0f) {
                float natural = static_cast<cocos2d::CCNode*>(l)->m_obContentSize.width;
                if (natural * scale > max_width) scale = max_width / natural;
            }
            if (left_align) {
                const cocos2d::CCPoint anchor{0.0f, 0.5f};
                detail::api().set_anchor(l, &anchor);
            }
            detail::api().set_scale(l, scale);
            place(l, x, y);
        }
        return *this;
    }

    // `scale` sizes the whole button; at the default 0.7 a short word is about 70 units wide,
    // so keep button centres at least ~90 units apart.
    // `height` is the button's own height (the sprite does not scale it down with `scale`), and `bg`
    // picks the colour: "GJ_button_01.png" green, "_02" blue, "_03" gray, "_04" pink/red. `width` > 0
    // makes it a fixed width in units (so a column of buttons lines up); 0 sizes it to the text.
    Popup& button(const char* text, float x, float y, std::function<void()> on_click, float scale = 0.7f,
                  float height = 30.0f, const char* bg = "GJ_button_01.png", float width = 0.0f) {
        if (!usable()) return *this;
        auto create = cadence::fn<static_cast<ButtonSprite*(*)(
            char const*, int, int, float, bool, char const*, char const*, float)>(&ButtonSprite::create)>();
        if (!create) return *this;
        ButtonSprite* sprite = width > 0.0f ? create(text, static_cast<int>(width), 0, scale, true, "goldFont.fnt", bg, height)
                     : create(text, 0, 0, scale, false, "goldFont.fnt", bg, height);
        CCMenuItemSpriteExtra* item = detail::make_item(sprite, [fn = std::move(on_click)](void*) { if (fn) fn(); });
        if (item) place(item, x, y);
        return *this;
    }

    // A button that is one of the game's sprite frames (e.g. "GJ_infoIcon_001.png",
    // "GJ_plus3Btn_001.png"), shrinking when pressed like the others.
    Popup& icon_button(const char* frame, float x, float y, std::function<void()> on_click, float scale = 1.0f,
                       bool flip_x = false) {
        if (!usable()) return *this;
        void* sprite = detail::api().frame_sprite(frame);
        if (!sprite) return *this;
        if (flip_x && detail::api().set_flip_x) detail::api().set_flip_x(sprite, true);     // e.g. a left arrow from a right one
        detail::api().set_scale(sprite, scale);
        CCMenuItemSpriteExtra* item = detail::make_item(sprite, [fn = std::move(on_click)](void*) { if (fn) fn(); });
        if (item) place(item, x, y);
        return *this;
    }

    // GD's checkbox with its label to the right (about 24 units tall at box_scale 0.8; the label
    // starts 28*box_scale units right of x). `max_label_width` > 0 shrinks long text to fit.
    // `on_change` gets the new state.
    Popup& checkbox(const char* text, float x, float y, bool initial, std::function<void(bool)> on_change,
                    float text_scale = 0.4f, float box_scale = 0.8f, float max_label_width = 0.0f) {
        if (!usable()) return *this;
        auto create = cadence::fn<&CCMenuItemToggler::create>();
        auto toggle = cadence::fn<&CCMenuItemToggler::toggle>();
        void* off = detail::api().frame_sprite("GJ_checkOff_001.png");
        void* on = detail::api().frame_sprite("GJ_checkOn_001.png");
        if (!create || !toggle || !off || !on) return *this;

        // Our own copy of the state: whether the game's callback sees the value before
        // or after its flip is not something to depend on.
        auto state = std::make_shared<bool>(initial);
        CCMenuItemToggler* item = create(static_cast<cocos2d::CCNode*>(off), static_cast<cocos2d::CCNode*>(on),
                                         static_cast<cocos2d::CCObject*>(detail::target_node()), detail::selector());
        if (!item) return *this;
        detail::handlers()[item] = [state, fn = std::move(on_change)](void*) {
            *state = !*state;
            if (fn) fn(*state);
        };
        toggle(item, initial);
        detail::api().set_scale(item, box_scale);
        place(item, x, y);
        label(text, x + 28.0f * box_scale, y, text_scale, "bigFont.fnt", true, max_label_width);
        return *this;
    }

    // Removes everything added with label/button/checkbox (not the title or Close).
    void clear() {
        if (!usable()) return;
        for (void* item : s_->items) {
            detail::handlers().erase(item);
            detail::api().remove_from_parent(item);
        }
        s_->items.clear();
    }

    bool show() {
        if (!s_->layer || s_->closed) return false;
        detail::show_layer(s_->layer);
        s_->shown = true;
        return true;
    }

    // Dismisses the popup as if the player had pressed back.
    void close() {
        if (!is_open()) return;
        // keyBackClicked is a method of a base class embedded in FLAlertLayer, so the game wants
        // `this` to point at that sub-object: the function starts with `lea rdi, [rcx - 0x150]`
        // to get back to the layer. Passing the layer itself crashes.
        constexpr size_t kKeypadBase = 0x150;
        using back_t = void(__fastcall*)(void*);
        if (auto back = reinterpret_cast<back_t>(cadence::addr(cadence::fn_sig<&FLAlertLayer::keyBackClicked>)))
            back(reinterpret_cast<char*>(s_->layer) + kKeypadBase);
        s_->closed = true;
    }

private:
    struct State {
        FLAlertLayer* layer = nullptr;
        float width = 0.0f, height = 0.0f;
        float shift_x = 0.0f, shift_y = 0.0f;
        bool shown = false;
        bool closed = false;
        std::vector<void*> items;
    };

    bool usable() const { return s_->layer && !s_->closed && s_->layer->m_buttonMenu && detail::api().ok(); }

    void place(void* node, float x, float y) {
        detail::api().set_position(node, x + s_->shift_x, y + s_->shift_y);
        detail::api().add_child(s_->layer->m_buttonMenu, node, 5);
        s_->items.push_back(node);
    }

    std::shared_ptr<State> s_;
};
} // namespace cadence::ui
