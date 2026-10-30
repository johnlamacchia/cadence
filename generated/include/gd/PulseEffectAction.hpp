#pragma once

class cocos2d;

class PulseEffectAction {
public:
    bool isFinished();
    void step(float delta);
    float valueForDelta(float currentTime, float fadeInTime, float holdTime, float fadeOutTime);

    bool m_disabled;
    float m_fadeInTime;
    float m_holdTime;
    float m_fadeOutTime;
    float m_deltaTime;
    int m_targetGroupID;
    float m_currentValue;
    cocos2d::ccColor3B m_color;
    PulseEffectType m_pulseEffectType;
    cocos2d::ccHSVValue m_hsv;
    int m_colorIndex;
    bool m_mainOnly;
    bool m_detailOnly;
    bool m_isDynamicHsv;
    int m_triggerUniqueID;
    int m_controlID;
    float m_startTime;

};
