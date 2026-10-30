#pragma once

class cocos2d;

class UIButtonConfig {
public:
    void reset();
    void resetOneBtn();

    int m_width;
    int m_height;
    float m_deadzone;
    float m_scale;
    int m_opacity;
    float m_radius;
    bool m_modeB;
    bool m_snap;
    cocos2d::CCPoint m_position;
    bool m_oneButton;
    bool m_player2;
    bool m_split;

};
