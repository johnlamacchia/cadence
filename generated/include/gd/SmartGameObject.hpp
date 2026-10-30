#pragma once

class GJBaseGameLayer;

class SmartGameObject : public GameObject {
public:
    virtual ~SmartGameObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SmartGameObject* create(char const* frame);

    SmartGameObject();
    bool init(char const* frame);
    void updateSmartFrame();

    bool m_referenceOnly;
    std::string m_baseFrame;
    std::string m_smartFrame;

};
