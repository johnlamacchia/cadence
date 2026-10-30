#pragma once

class GJBaseGameLayer;
class GameObject;

class CameraTriggerGameObject : public EffectGameObject {
public:
    virtual ~CameraTriggerGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static CameraTriggerGameObject* create(char const* frame);

    CameraTriggerGameObject();
    bool init(char const* frame);

    bool m_exitStatic;
    bool m_followObject;
    float m_followEasing;
    int m_edgeDirection;
    bool m_smoothVelocity;
    float m_velocityModifier;
    bool m_exitInstant;
    float m_previewOpacity;

};
