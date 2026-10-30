#pragma once

class GameObject;
class SFXTriggerGameObject;

class SFXTriggerInstance {
public:
    int m_groupID1;
    int m_groupID2;
    int m_controlID;
    SFXTriggerGameObject* m_sfxTriggerGameObject;

};
