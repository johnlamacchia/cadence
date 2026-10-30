#pragma once

class cocos2d;

class CCSpriteCOpacity : public cocos2d::CCSprite {
public:
    virtual void setOpacity(unsigned char opacity);

    static CCSpriteCOpacity* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame);
    static CCSpriteCOpacity* createWithSpriteFrameName(char const* frameName);

    CCSpriteCOpacity();

};
