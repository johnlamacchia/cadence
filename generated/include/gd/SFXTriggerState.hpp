#pragma once

class GameObject;
class SFXStateContainer;
class SFXTriggerGameObject;

class SFXTriggerState {
public:
    SFXTriggerGameObject* m_sfxTriggerGameObject;
    int m_instanceID;
    double m_startTime;
    double m_playPosition;
    double m_positionTime;
    double m_lastUpdateTime;
    float m_speed;
    float m_groupSpeed;
    float m_volume;
    float m_length;
    int m_index;
    bool m_processed;
    bool m_loop;
    std::array<SFXStateContainer, 3> m_sfxStateContainers;

};
