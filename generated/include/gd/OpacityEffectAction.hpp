#pragma once

class OpacityEffectAction {
public:
    void step(float delta);

    float m_duration;
    float m_fromValue;
    float m_toValue;
    bool m_finished;
    bool m_disabled;
    float m_deltaTime;
    int m_targetGroupID;
    float m_currentValue;
    int m_triggerUniqueID;
    int m_controlID;
    float m_deltaTimeRelated;
    float m_durationRelated;

};
