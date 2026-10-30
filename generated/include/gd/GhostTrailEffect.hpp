#pragma once

class PlayerObject;
class cocos2d;

class GhostTrailEffect : public cocos2d::CCNode {
public:
    virtual ~GhostTrailEffect();
    virtual bool init();
    virtual void draw();

    static GhostTrailEffect* create();

    void doBlendAdditive();
    void runWithTarget(cocos2d::CCSprite* sprite, float snapshotInterval, float fadeInterval, float duration, float ghostScale, bool scaleTwice);
    void stopTrail();
    void trailSnapshot(float dt);

    float m_snapshotInterval;
    float m_fadeInterval;
    float m_ghostScale;
    bool m_scaleTwice;
    float m_playerScale;
    cocos2d::ccBlendFunc m_blendFunc;
    cocos2d::CCSprite* m_iconSprite;
    PlayerObject* m_playerObject;
    cocos2d::CCLayer* m_objectLayer;
    float m_opacity;
    void* m_delegate;
    cocos2d::ccColor3B m_color;
    cocos2d::CCPoint m_position;
    bool m_unk194;

};
