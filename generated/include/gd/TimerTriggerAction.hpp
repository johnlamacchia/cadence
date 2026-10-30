#pragma once

class TimerTriggerAction {
public:
    TimerTriggerAction();
    TimerTriggerAction(int targetID, float targetTime, bool multiActivate, int itemID, std::vector<int> const& remapKeys, int uniqueID, int controlID);

    bool m_disabled;
    float m_time;
    float m_targetTime;
    int m_targetGroupID;
    int m_triggerUniqueID;
    int m_controlID;
    int m_itemID;
    bool m_multiActivate;
    std::vector<int> m_remapKeys;

};
