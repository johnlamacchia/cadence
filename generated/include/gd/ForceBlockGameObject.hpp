#pragma once

class GJBaseGameLayer;
class GameObject;
class cocos2d;

class ForceBlockGameObject : public EffectGameObject {
public:
    virtual ~ForceBlockGameObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static ForceBlockGameObject* create(char const* frame);

    ForceBlockGameObject();
    cocos2d::CCPoint calculateForceToTarget(GameObject* target);
    bool init(char const* frame);

    float m_force;
    float m_minForce;
    float m_maxForce;
    bool m_relativeForce;
    bool m_forceRange;
    int m_forceID;

};
