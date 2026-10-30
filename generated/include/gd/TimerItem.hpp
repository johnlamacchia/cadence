#pragma once

class TimerItem {
public:
    int m_itemID;
    double m_time;
    bool m_active;
    float m_timeMod;
    bool m_ignoreTimeWarp;
    double m_targetTime;
    bool m_stopTimeEnabled;
    int m_targetGroupID;
    int m_triggerUniqueID;
    int m_controlID;
    std::vector<int> m_remapKeys;
    bool m_disabled;

};
