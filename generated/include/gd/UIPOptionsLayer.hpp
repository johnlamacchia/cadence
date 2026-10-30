#pragma once

class OptionsLayer;
class cocos2d;

class UIPOptionsLayer : public SetupTriggerPopup {
public:
    virtual ~UIPOptionsLayer();
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual float getValue(int tag);

    static UIPOptionsLayer* create();

    UIPOptionsLayer();
    cocos2d::CCRect getTouchRect();
    void onReset(cocos2d::CCObject* sender);

    int m_touchID;
    cocos2d::CCNode* m_practiceNode;
    cocos2d::CCPoint m_touchPosition;
    cocos2d::CCPoint m_touchDelta;

};
