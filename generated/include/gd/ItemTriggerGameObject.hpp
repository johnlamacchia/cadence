#pragma once

class GJBaseGameLayer;
class GameObject;

class ItemTriggerGameObject : public EffectGameObject {
public:
    virtual ~ItemTriggerGameObject();
    virtual void customSetup();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static ItemTriggerGameObject* create(char const* frame);

    ItemTriggerGameObject();
    bool init(char const* frame);

    int m_item1Mode;
    int m_item2Mode;
    int m_targetItemMode;
    float m_mod1;
    float m_mod2;
    int m_resultType1;
    int m_resultType2;
    int m_resultType3;
    float m_tolerance;
    int m_roundType1;
    int m_roundType2;
    int m_signType1;
    int m_signType2;
    bool m_persistent;
    bool m_targetAll;
    bool m_reset;
    bool m_timer;

};
