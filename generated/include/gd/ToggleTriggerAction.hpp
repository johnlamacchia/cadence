#pragma once

class ToggleTriggerAction {
public:
    ToggleTriggerAction();
    ToggleTriggerAction(int targetID, bool activateGroup, std::vector<int> const& remapKeys, int uniqueID, int controlID);

    bool m_disabled;
    int m_targetGroupID;
    bool m_activateGroup;
    int m_triggerUniqueID;
    int m_controlID;
    std::vector<int> m_remapKeys;

};
