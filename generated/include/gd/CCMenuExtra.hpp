#pragma once

class cocos2d;

class CCMenuExtra : public cocos2d::CCMenu {
public:
    virtual ~CCMenuExtra();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);

    static CCMenuExtra* create();

    void /* TodoReturn */ disableRepeatPress();
    void /* TodoReturn */ enableRepeatPress();
    void /* TodoReturn */ enableRepeatPress(float, float);
    void /* TodoReturn */ fireSelectedItem();
    void /* TodoReturn */ repeatSelectionChanged();
    void /* TodoReturn */ repeatTick(float);
    void /* TodoReturn */ stopRepeat();

};
