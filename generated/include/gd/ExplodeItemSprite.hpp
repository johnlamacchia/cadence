#pragma once

class cocos2d;

class ExplodeItemSprite : public cocos2d::CCSprite {
public:
    virtual ~ExplodeItemSprite();
    virtual bool init();

    static ExplodeItemSprite* create();

    ExplodeItemSprite();

    float m_xVelocity;
    float m_yVelocity;
    float m_timeRemaining;
    float m_fadeOutTime;
    float m_rotVelocity;
    cocos2d::CCParticleSystemQuad* m_particles;

};
