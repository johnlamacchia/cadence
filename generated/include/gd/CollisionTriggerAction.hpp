#pragma once

class CollisionTriggerAction {
public:
    bool m_disabled;
    int m_blockAID;
    int m_blockBID;
    int m_targetGroupID;
    int m_triggerOnExit;
    bool m_activateGroup;
    int m_triggerUniqueID;
    int m_controlID;
    std::vector<int> m_remapKeys;

};
