#pragma once

class LoadingCircle;
class cocos2d;

class LoadingCircleSprite : public cocos2d::CCSprite {
public:
    virtual ~LoadingCircleSprite();

    static LoadingCircleSprite* create(float spinSpeed);

    cocos2d::CCAction* fadeInCircle(bool resetOpacity, float duration, float opacity);
    void hideCircle();
    bool init(float spinSpeed);

};
