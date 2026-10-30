#pragma once

class CCTextInputNode;
class cocos2d;

class CCTextInputMenu : public cocos2d::CCLayer {
public:
    virtual ~CCTextInputMenu();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual bool rightClick(float, float);

    static CCTextInputMenu* create(CCTextInputNode*, cocos2d::CCPoint);

    void /* TodoReturn */ closeDeferred(float);
    void /* TodoReturn */ closeNextFrame();
    bool init(CCTextInputNode*, cocos2d::CCPoint);
    void onClear(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);
    void onPaste(cocos2d::CCObject* sender);

};
