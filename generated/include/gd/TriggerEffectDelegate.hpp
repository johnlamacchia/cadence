#pragma once

class GameObject;

class TriggerEffectDelegate {
public:
    virtual void toggleGroupTriggered(int group, bool activate, std::vector<int> const& remapKeys, int triggerID, int controlID);
    virtual void spawnGroup(int group, bool ordered, double delay, std::vector<int> const& remapKeys, int triggerID, int controlID);
    virtual void spawnObject(GameObject* object, double delay, std::vector<int> const& remapKeys);
    virtual bool checkSpawnAbuse();

};
