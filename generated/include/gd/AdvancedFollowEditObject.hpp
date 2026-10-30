#pragma once

class GJBaseGameLayer;

class AdvancedFollowEditObject : public AdvancedFollowTriggerObject {
public:
    virtual ~AdvancedFollowEditObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static AdvancedFollowEditObject* create(char const* frame);

    AdvancedFollowEditObject();
    bool init(char const* frame);

    float m_modX;
    float m_modXVariance;
    float m_modY;
    float m_modYVariance;
    bool m_redirectDirection;

};
