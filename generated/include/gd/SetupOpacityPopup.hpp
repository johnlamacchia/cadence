#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupOpacityPopup : public SetupTriggerPopup {
public:
    virtual ~SetupOpacityPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static SetupOpacityPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateDuration();
    void updateDurLabel(bool decimals);
    void updateOpacity();
    void updateOpacityLabel();
    void updateTargetID();
    void updateTextInputLabel();

    CCTextInputNode* m_groupIDInput;
    CCTextInputNode* m_fadeTimeInput;
    cocos2d::CCLabelBMFont* m_opacityLabel;
    Slider* m_fadeTimeSlider;
    Slider* m_opacitySlider;
    float m_opacity;
    float m_fadeTime;
    int m_groupID;

};
