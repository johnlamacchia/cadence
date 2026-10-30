#pragma once

class GJBaseGameLayer;
class GameObject;
class ParticleGameObject;
class cocos2d;

class SpawnParticleGameObject : public EffectGameObject {
public:
    virtual ~SpawnParticleGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SpawnParticleGameObject* create();

    SpawnParticleGameObject();

    cocos2d::CCPoint m_offset;
    cocos2d::CCPoint m_offsetVariance;
    bool m_matchRotation;
    float m_rotation;
    float m_rotationVariance;
    float m_scale;
    float m_scaleVariance;

};
