#pragma once

class GJBaseGameLayer;
class GameObject;

class PlayerControlGameObject : public EffectGameObject {
public:
    virtual ~PlayerControlGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static PlayerControlGameObject* create();

    PlayerControlGameObject();

    bool m_stopJump;
    bool m_stopMove;
    bool m_stopRotation;
    bool m_stopSlide;

};
