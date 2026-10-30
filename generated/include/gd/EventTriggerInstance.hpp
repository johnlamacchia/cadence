#pragma once

class EventTriggerInstance {
public:
    int m_targetID;
    int m_uniqueID;
    int m_controlID;
    bool m_inactive;
    std::vector<int> m_remapKeys;

};
