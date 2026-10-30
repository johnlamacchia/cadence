#pragma once

class UIButtonConfig;
class cocos2d;

class LevelTouchSlot {
public:
    UIButtonConfig m_config;
    cocos2d::CCPoint m_offset;
    int m_anchorX;
    int m_anchorY;
    bool m_scaleX;
    bool m_scaleY;

};
