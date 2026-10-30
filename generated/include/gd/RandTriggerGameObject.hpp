#pragma once

class GJBaseGameLayer;
class GameObject;

class RandTriggerGameObject : public ChanceTriggerGameObject {
public:
    virtual ~RandTriggerGameObject();
    virtual bool init();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static RandTriggerGameObject* create();

    RandTriggerGameObject();
    int getRandomGroupID();
    int getTotalChance();

};
