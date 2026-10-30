#pragma once

class GJValueTween {
public:
    void step(float delta);

    float m_fromValue;
    float m_toValue;
    float m_duration;
    float m_deltaTime;
    float m_currentValue;
    int m_easingType;
    float m_easingRate;
    bool m_finished;
    bool m_disabled;
    int m_uniqueID;
    int m_controlID;

};
