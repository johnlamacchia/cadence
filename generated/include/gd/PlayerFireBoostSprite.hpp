#pragma once

class PlayerFireBoostSprite : public cocos2d::CCSprite {
public:
    virtual ~PlayerFireBoostSprite();
    virtual bool init();

    static PlayerFireBoostSprite* create();

    void animateFireIn();
    void animateFireOut();
    void loopFireAnimation();

    float m_size;

};
