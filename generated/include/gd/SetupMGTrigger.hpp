#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class ConfigureValuePopup;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupMGTrigger : public SetupTriggerPopup {
public:
    virtual ~SetupMGTrigger();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);
    virtual void valuePopupClosed(ConfigureValuePopup* popup, float value);

    static SetupMGTrigger* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onEasing(cocos2d::CCObject* sender);
    void onEasingRate(cocos2d::CCObject* sender);
    int posFromSliderValue(float value);
    void sliderChanged(cocos2d::CCObject* sender);
    float sliderValueFromPos(int pos);
    void sliderYChanged(cocos2d::CCObject* sender);
    void toggleEasingRateVisibility();
    void updateDuration();
    void updateDurLabel(bool decimals);
    void updateEasingLabel();
    void updateEasingRateLabel();
    void updateMoveCommandEasing();
    void updateMoveCommandEasingRate();
    void updateMoveCommandPosY();
    void updateValueYLabel();

    CCTextInputNode* m_moveTimeInput;
    Slider* m_moveTimeSlider;
    float m_moveTime;
    cocos2d::CCLabelBMFont* m_mgEasingLabel;
    EasingType m_mgEasingType;
    float m_mgEasingRate;
    CCMenuItemSpriteExtra* m_mgEasingRateButton;
    cocos2d::CCLabelBMFont* m_mgEasingRateLabel;
    CCTextInputNode* m_offsetYInput;
    Slider* m_offsetYSlider;
    int m_offsetY;

};
