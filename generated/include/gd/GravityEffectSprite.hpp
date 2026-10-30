#pragma once

class cocos2d;

class GravityEffectSprite : public cocos2d::CCSprite {
public:
    virtual ~GravityEffectSprite();
    virtual bool init();
    virtual void draw();

    static GravityEffectSprite* create();

    GravityEffectSprite();
    void updateSpritesColor(cocos2d::ccColor3B color);

};
