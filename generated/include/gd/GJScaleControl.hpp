#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class ControlMenuTouch;
class GJScaleControlDelegate;
class GameObject;
class GameObjectEditorState;
class Slider;
class cocos2d;

class GJScaleControl : public cocos2d::CCLayer {
public:
    virtual ~GJScaleControl();
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static GJScaleControl* create();

    GJScaleControl();
    void /* TodoReturn */ applyFocusedInput();
    void /* TodoReturn */ createScaleInput(cocos2d::CCPoint, int, CCMenuItemSpriteExtra**);
    void finishTouch();
    void /* TodoReturn */ hasActiveInput();
    void loadValues(GameObject* object, cocos2d::CCArray* objects, std::unordered_map<int, GameObjectEditorState>& editorStates);
    void onApplyScale(cocos2d::CCObject* sender);
    void onToggleLockScale(cocos2d::CCObject* sender);
    void /* TodoReturn */ releaseInputFocus();
    float scaleFromValue(float value);
    float skewFromValue(float value);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateLabelX(float value);
    void updateLabelXY(float value);
    void updateLabelY(float value);
    float valueFromScale(float scale);
    float valueFromSkew(float value);

    Slider* m_sliderX;
    Slider* m_sliderY;
    Slider* m_sliderXY;
    int m_touchID;
    float m_valueX;
    float m_valueY;
    int m_unkSize4;
    float m_changedValueX;
    float m_changedValueY;
    bool m_scale1Lock;
    ScaleButtonType m_scaleButtonType;
    cocos2d::CCLabelBMFont* m_scaleXLabel;
    cocos2d::CCLabelBMFont* m_scaleYLabel;
    cocos2d::CCLabelBMFont* m_scaleLabel;
    CCTextInputNode* m_scaleXInput;
    CCTextInputNode* m_scaleYInput;
    CCTextInputNode* m_scaleInput;
    CCMenuItemSpriteExtra* m_applyScaleXButton;
    CCMenuItemSpriteExtra* m_applyScaleYButton;
    CCMenuItemSpriteExtra* m_applyScaleButton;
    cocos2d::CCMenu* m_controlMenu;
    ControlMenuTouch m_controlMenuTouch;
    GJScaleControlDelegate* m_delegate;
    float m_upperBound;
    float m_lowerBound;
    int m_senderTag;
    CCMenuItemSpriteExtra* m_scaleLockButton;
    bool m_scaleLocked;

};
