#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class CCTextInputNode;
class CameraTriggerGameObject;
class ConfigureValuePopup;
class GameObject;
class Slider;
class cocos2d;

class SetupCameraOffsetTrigger : public SetupTriggerPopup {
public:
    virtual ~SetupCameraOffsetTrigger();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);
    virtual void valuePopupClosed(ConfigureValuePopup* popup, float value);

    static SetupCameraOffsetTrigger* create(CameraTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(CameraTriggerGameObject* object, cocos2d::CCArray* objects);
    void onEasing(cocos2d::CCObject* sender);
    void onEasingRate(cocos2d::CCObject* sender);
    void onTargetMode(cocos2d::CCObject* sender);
    int posFromSliderValue(float value);
    void sliderChanged(cocos2d::CCObject* sender);
    float sliderValueFromPos(int pos);
    void sliderXChanged(cocos2d::CCObject* sender);
    void sliderYChanged(cocos2d::CCObject* sender);
    void toggleEasingRateVisibility();
    void updateDuration();
    void updateDurLabel(bool decimals);
    void updateEasingLabel();
    void updateEasingRateLabel();
    void updateMoveCommandEasing();
    void updateMoveCommandEasingRate();
    void updateMoveCommandPosX();
    void updateMoveCommandPosY();
    void updateValueXLabel();
    void updateValueYLabel();

    CCTextInputNode* m_moveTimeInput;
    Slider* m_moveTimeSlider;
    float m_moveTime;
    cocos2d::CCLabelBMFont* m_cameraEasingLabel;
    EasingType m_cameraEasingType;
    float m_cameraEasingRate;
    CCMenuItemSpriteExtra* m_cameraEasingRateButton;
    cocos2d::CCLabelBMFont* m_cameraEasingRateLabel;
    CCTextInputNode* m_offsetXInput;
    CCTextInputNode* m_offsetYInput;
    Slider* m_offsetXSlider;
    Slider* m_offsetYSlider;
    int m_offsetX;
    int m_offsetY;
    int m_targetMode;
    CCMenuItemToggler* m_xOnlyToggler;
    CCMenuItemToggler* m_yOnlyToggler;

};
