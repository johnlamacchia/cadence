#pragma once

class GJUINode;
class LevelControlConfig;
class cocos2d;

class TouchControlEditLayer : public SetupTriggerPopup {
public:
    virtual ~TouchControlEditLayer();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void valueDidChange(int, float);
    virtual float getValue(int);

    static TouchControlEditLayer* create(bool, LevelControlConfig*);

    void /* TodoReturn */ activeNode();
    void /* TodoReturn */ beginDeadzoneDrag(GJUINode*, cocos2d::CCPoint const&);
    void /* TodoReturn */ buildHud();
    void /* TodoReturn */ buildNodes();
    void /* TodoReturn */ clearNodes();
    void /* TodoReturn */ commitToTarget();
    void /* TodoReturn */ copyTouchFrom(LevelControlConfig*);
    void /* TodoReturn */ globalDpad(int);
    void /* TodoReturn */ hideSiblings();
    bool init(bool, LevelControlConfig*);
    void onApplyGlobal(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);
    void onOK(cocos2d::CCObject* sender);
    void onPaste(cocos2d::CCObject* sender);
    void onReset(cocos2d::CCObject* sender);
    void onSlots(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshHud();
    void /* TodoReturn */ reloadNodes();
    void /* TodoReturn */ restoreSiblings();
    void /* TodoReturn */ saveTarget();
    void /* TodoReturn */ updateHandles();
    void /* TodoReturn */ updateHudVisibility();
    void /* TodoReturn */ updateJumpGuide();

};
