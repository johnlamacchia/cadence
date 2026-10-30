#pragma once

class AdvancedFollowTriggerObject;

class AdvancedFollowInstance {
public:
    AdvancedFollowInstance();
    AdvancedFollowInstance(AdvancedFollowTriggerObject* object, int groupID, int targetID, int otherID, int unknown, int controlID);

    AdvancedFollowTriggerObject* m_gameObject;
    int m_group;
    int m_objectKey;
    int m_controlId;
    int m_otherObjectKey;
    int m_relatedToGJGameStateUnkUint7;
    bool m_finished;
    bool m_doStart;
    bool m_started;
    bool m_processed;

};
