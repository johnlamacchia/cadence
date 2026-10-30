#pragma once

class ChanceObject;
class GJBaseGameLayer;
class GameObject;

class SpawnTriggerGameObject : public EffectGameObject {
public:
    virtual ~SpawnTriggerGameObject();
    virtual bool init();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SpawnTriggerGameObject* create();

    SpawnTriggerGameObject();
    void addRemap(int oldID, int newID);
    void changeRemap(int oldID, int newID, bool reverse);
    ChanceObject* getRemapWithSource(int id);
    ChanceObject* getRemapWithTarget(int id);
    void removeRemap(int oldID, int newID);
    void updateRemapKeys(std::vector<int> const& remapKeys);

    std::vector<ChanceObject> m_remapObjects;
    int m_remapKey;
    std::vector<int> m_remapKeys;
    double m_currentDelay;
    float m_spawnDelay;
    float m_delayRange;
    bool m_resetRemap;

};
