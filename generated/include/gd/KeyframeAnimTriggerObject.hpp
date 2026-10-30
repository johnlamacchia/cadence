#pragma once

class GJBaseGameLayer;

class KeyframeAnimTriggerObject : public EffectGameObject {
public:
    virtual ~KeyframeAnimTriggerObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static KeyframeAnimTriggerObject* create();

    KeyframeAnimTriggerObject();

    float m_timeMod;
    float m_positionXMod;
    float m_positionYMod;
    float m_rotationMod;
    float m_scaleXMod;
    float m_scaleYMod;

};
