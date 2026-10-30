#pragma once

class GJBaseGameLayer;
class GameObject;

class RotateGameplayGameObject : public EffectGameObject {
public:
    virtual ~RotateGameplayGameObject();
    virtual bool init();
    virtual void updateStartValues();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static RotateGameplayGameObject* create();

    RotateGameplayGameObject();
    void updateGameplayRotation();

    int m_moveDirection;
    int m_groundDirection;
    bool m_editVelocity;
    bool m_overrideVelocity;
    float m_velocityModX;
    float m_velocityModY;
    bool m_changeChannel;
    bool m_channelOnly;
    int m_targetChannelID;
    bool m_instantOffset;
    bool m_dontSlide;

};
