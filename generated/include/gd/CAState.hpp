#pragma once

class cocos2d;

class CAState {
public:
    cocos2d::ccColor3B m_fromColor;
    cocos2d::ccColor3B m_toColor;
    cocos2d::ccColor3B m_color;
    bool m_paused;
    bool m_blending;
    bool m_copyOpacity;
    bool m_legacyHSV;
    int m_playerColor;
    int m_colorID;
    int m_copyID;
    int m_uniqueID;
    float m_duration;
    float m_fromOpacity;
    float m_toOpacity;
    float m_deltaTime;
    float m_currentOpacity;
    cocos2d::ccHSVValue m_copyHSV;

};
