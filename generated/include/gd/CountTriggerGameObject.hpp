#pragma once

class GJBaseGameLayer;
class GameObject;

class CountTriggerGameObject : public EffectGameObject {
public:
    virtual ~CountTriggerGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static CountTriggerGameObject* create(char const* frame);

    CountTriggerGameObject();
    bool init(char const* frame);

    int m_pickupCount;
    int m_pickupTriggerMode;
    bool m_multiActivate;
    bool m_isOverride;
    float m_pickupTriggerMultiplier;

};
