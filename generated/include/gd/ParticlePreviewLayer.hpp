#pragma once

class cocos2d;

class ParticlePreviewLayer : public cocos2d::CCLayerColor {
public:
    virtual ~ParticlePreviewLayer();
    virtual void draw();
    virtual void visit();

    static ParticlePreviewLayer* create(cocos2d::CCParticleSystemQuad* particleSystem);

    ParticlePreviewLayer();
    bool init(cocos2d::CCParticleSystemQuad* particleSystem);
    void postVisit();
    void preVisitWithClippingRect(cocos2d::CCRect rect);

    int m_particleMode;
    int m_drawMode;
    cocos2d::CCParticleSystemQuad* m_particleSystem;
    bool m_gravityMode;

};
