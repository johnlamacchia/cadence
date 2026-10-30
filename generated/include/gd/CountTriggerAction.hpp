#pragma once

class CountTriggerAction {
public:
    CountTriggerAction();
    CountTriggerAction(int targetID, int targetCount, bool activateGroup, bool multiActivate, int itemID, int previousCount, std::vector<int> const& remapKeys, int uniqueID, int controlID);

    bool m_disabled;
    int m_previousCount;
    int m_targetCount;
    int m_targetGroupID;
    bool m_activateGroup;
    int m_triggerUniqueID;
    int m_controlID;
    int m_itemID;
    bool m_multiActivate;
    std::vector<int> m_remapKeys;

};
