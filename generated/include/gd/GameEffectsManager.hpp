#pragma once

class PlayLayer;
class cocos2d;

class GameEffectsManager : public cocos2d::CCNode {
public:
    static GameEffectsManager* create(PlayLayer* playLayer);

    void addParticleEffect(cocos2d::CCParticleSystemQuad* particle, int unused);
    bool init(PlayLayer* playLayer);
    void scaleParticle(cocos2d::CCParticleSystemQuad* particle, float scale);

    PlayLayer* m_playLayer;

};
