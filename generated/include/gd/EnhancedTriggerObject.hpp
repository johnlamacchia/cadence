#pragma once

class GJBaseGameLayer;

class EnhancedTriggerObject : public EffectGameObject {
public:
    virtual ~EnhancedTriggerObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static EnhancedTriggerObject* create(char const* frame);

    EnhancedTriggerObject();
    bool init(char const* frame);

    int m_minXID;
    int m_minYID;
    int m_maxXID;
    int m_maxYID;

};
