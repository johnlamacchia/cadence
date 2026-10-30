#pragma once

class cocos2d;

class KeybindSettingsEntry {
public:
    char const* m_label;
    char const* m_sprite;
    cocos2d::CCObject* m_target;
    cocos2d::SEL_MenuHandler m_callback;
    bool m_closePopup;

};
