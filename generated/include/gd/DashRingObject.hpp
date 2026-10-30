#pragma once

class GJBaseGameLayer;

class DashRingObject : public RingObject {
public:
    virtual ~DashRingObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static DashRingObject* create(char const* frame);

    DashRingObject();
    bool init(char const* frame);

    float m_dashSpeed;
    float m_endBoost;
    float m_maxDuration;
    bool m_allowCollide;
    bool m_stopSlide;

};
