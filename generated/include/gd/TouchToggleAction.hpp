#pragma once

class TouchToggleAction {
public:
    TouchToggleAction();
    TouchToggleAction(int targetID, bool holdMode, TouchTriggerType touchType, TouchTriggerControl touchControl, bool dualMode, std::vector<int> const& remapKeys, int uniqueID, int controlID);

    bool m_disabled;
    int m_targetGroupID;
    bool m_holdMode;
    TouchTriggerType m_touchTriggerType;
    TouchTriggerControl m_touchTriggerControl;
    int m_triggerUniqueID;
    int m_controlID;
    bool m_dualMode;
    std::vector<int> m_remapKeys;

};
