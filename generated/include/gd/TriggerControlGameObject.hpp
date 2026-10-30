#pragma once

class GJBaseGameLayer;
class GameObject;

class TriggerControlGameObject : public EffectGameObject {
public:
    virtual ~TriggerControlGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static TriggerControlGameObject* create(char const* frame);

    TriggerControlGameObject();
    bool init(char const* frame);
    void updateTriggerControlFrame();

    std::string m_triggerControlFrame;
    GJActionCommand m_customTriggerValue;

};
