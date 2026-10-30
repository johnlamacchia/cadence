#pragma once

class AdvFollowSetup {
public:
    std::unordered_set<int> m_centerIDs;
    std::unordered_set<int> m_groupIDs;
    std::unordered_set<int> m_controlIDs;
    int m_delay;

};
