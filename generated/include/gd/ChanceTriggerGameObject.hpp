#pragma once

class ChanceObject;

class ChanceTriggerGameObject : public EffectGameObject {
public:
    ChanceTriggerGameObject();
    bool containsTargetID(int id);
    void editChanceObject(int oldID, int newID);
    void getTargetIDs(std::vector<int>& ids);
    bool init(char const* frame);
    void remapChanceObjects(std::unordered_map<int, int> const* remap);
    void revertChanceRemap();

    std::vector<ChanceObject> m_chanceObjects;

};
