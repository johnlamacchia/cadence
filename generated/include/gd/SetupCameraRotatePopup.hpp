#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class ConfigureValuePopup;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupCameraRotatePopup : public SetupTriggerPopup {
public:
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);
    virtual void valuePopupClosed(ConfigureValuePopup* popup, float value);

    static SetupCameraRotatePopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onEasing(cocos2d::CCObject* sender);
    void onEasingRate(cocos2d::CCObject* sender);
    void sliderChanged(cocos2d::CCObject* sender);
    void sliderDegreesChanged(cocos2d::CCObject* sender);
    void toggleEasingRateVisibility();
    void updateCommandDegrees();
    void updateDegreesLabel();
    void updateDuration();
    void updateDurLabel(bool decimals);
    void updateEasingLabel();
    void updateEasingRateLabel();
    void updateMoveCommandEasing();
    void updateMoveCommandEasingRate();

    CCTextInputNode* m_moveTimeInput;
    Slider* m_moveTimeSlider;
    float m_moveTime;
    cocos2d::CCLabelBMFont* m_cameraEasingLabel;
    EasingType m_cameraEasingType;
    float m_cameraEasingRate;
    CCMenuItemSpriteExtra* m_cameraEasingRateButton;
    cocos2d::CCLabelBMFont* m_cameraEasingRateLabel;
    CCTextInputNode* m_degreesInput;
    Slider* m_degreesSlider;
    float m_degrees;

};
