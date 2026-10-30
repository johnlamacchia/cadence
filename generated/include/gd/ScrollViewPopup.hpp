#pragma once

class cocos2d;

class ScrollViewPopup : public FLAlertLayer {
public:
    virtual ~ScrollViewPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();

    void /* TodoReturn */ addVersionHeader(cocos2d::CCNode*, int, int, float, float);
    void /* TodoReturn */ initPopup(cocos2d::CCArray*, char const*, char const*, float, float);
    void onClose(cocos2d::CCObject* sender);

};
