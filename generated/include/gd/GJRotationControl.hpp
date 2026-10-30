#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class ControlMenuTouch;
class GJRotationControlDelegate;
class cocos2d;

class GJRotationControl : public cocos2d::CCLayer {
public:
    virtual ~GJRotationControl();
    virtual bool init();
    virtual void draw();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static GJRotationControl* create();

    GJRotationControl();
    void /* TodoReturn */ applyFocusedInput();
    void finishTouch();
    void /* TodoReturn */ hasActiveInput();
    void onApplyAngle(cocos2d::CCObject* sender);
    void onToggleLockRotation(cocos2d::CCObject* sender);
    void /* TodoReturn */ releaseInputFocus();
    void setAngle(float angle);
    void /* TodoReturn */ updateAngleInput(float);
    void updateSliderPosition(cocos2d::CCPoint position);

    cocos2d::CCPoint m_cursorDifference;
    cocos2d::CCPoint m_controlPosition;
    cocos2d::CCSprite* m_controlSprite;
    float m_startingRotation;
    float m_currentRotation;
    int m_touchID;
    CCMenuItemSpriteExtra* m_lockRotationButton;
    bool m_rotationLocked;
    CCTextInputNode* m_angleInput;
    CCMenuItemSpriteExtra* m_applyAngleButton;
    ControlMenuTouch m_controlMenuTouch;
    GJRotationControlDelegate* m_delegate;

};
