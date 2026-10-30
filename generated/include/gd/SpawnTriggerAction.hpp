#pragma once

class GameObject;

class SpawnTriggerAction {
public:
    SpawnTriggerAction();
    SpawnTriggerAction(GameObject* object, float delay, std::vector<int> const& remapKeys, int uniqueID, int controlID);
    SpawnTriggerAction(int targetID, float delay, bool spawnOrdered, std::vector<int> const& remapKeys, int uniqueID, int controlID);
    bool isFinished();
    void step(float dt);

    bool m_finished;
    bool m_disabled;
    double m_duration;
    double m_deltaTime;
    int m_targetGroupID;
    int m_triggerUniqueID;
    int m_controlID;
    bool m_spawnOrdered;
    GameObject* m_gameObject;
    std::vector<int> m_remapKeys;

};
