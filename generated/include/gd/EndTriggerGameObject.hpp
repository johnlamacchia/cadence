#pragma once

class GJBaseGameLayer;
class GameObject;

class EndTriggerGameObject : public EffectGameObject {
public:
    virtual ~EndTriggerGameObject();
    virtual bool init();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static EndTriggerGameObject* create();

    EndTriggerGameObject();

    bool m_noEffects;
    bool m_noSFX;
    bool m_instant;

};
