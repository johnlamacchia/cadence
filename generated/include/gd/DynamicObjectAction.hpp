#pragma once

class EffectGameObject;
class GameObject;

class DynamicObjectAction {
public:
    DynamicObjectAction();
    DynamicObjectAction(EffectGameObject* object, GameObject* targetObject, GameObject* centerObject);

    EffectGameObject* m_trigger;
    GameObject* m_centerObject;
    GameObject* m_aimTargetObject;
    GameObject* m_targetObject;
    GameObject* m_minXObject;
    GameObject* m_minYObject;
    GameObject* m_maxXObject;
    GameObject* m_maxYObject;
    float m_elapsed;
    float m_easedElapsed;
    float m_easedProgress;
    bool m_justStarted;
    bool m_stopped;
    bool m_paused;
    bool m_pausePending;
    int m_controlID;
    int m_targetGroupID;
    int m_centerGroupID;

};
