#pragma once

class GJBaseGameLayer;
class GameObject;

class TransformTriggerGameObject : public EffectGameObject {
public:
    virtual ~TransformTriggerGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static TransformTriggerGameObject* create(char const* frame);

    TransformTriggerGameObject();
    bool init(char const* frame);

    float m_objectScaleX;
    float m_objectScaleY;
    float m_property450;
    float m_property451;
    bool m_onlyMove;
    bool m_divideX;
    bool m_divideY;
    bool m_relativeRotation;
    bool m_relativeScale;

};
