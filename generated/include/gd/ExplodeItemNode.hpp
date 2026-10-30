#pragma once

class cocos2d;

class ExplodeItemNode : public cocos2d::CCNode {
public:
    virtual ~ExplodeItemNode();
    virtual void update(float dt);

    static ExplodeItemNode* create(cocos2d::CCRenderTexture* texture);

    void createSprites(int countX, int countY, float xVel, float xVar, float yVel, float yVar, float dur, float durVar, cocos2d::ccColor4F startColor, cocos2d::ccColor4F endColor, bool noParticles);
    bool init(cocos2d::CCRenderTexture* texture);

    cocos2d::CCRenderTexture* m_renderTexture;
    cocos2d::CCArray* m_sprites;
    float m_unk1;
    float m_floorHeight;
    bool m_noGravity;

};
