#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupShakePopup : public SetupTriggerPopup {
public:
    virtual ~SetupShakePopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupShakePopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateDuration();
    void updateDurLabel(bool decimals);
    void updateInterval();
    void updateIntervalLabel(bool decimals);
    void updateShake();
    void updateShakeLabel(bool decimals);

    CCTextInputNode* m_durationInput;
    Slider* m_durationSlider;
    float m_duration;
    CCTextInputNode* m_strengthInput;
    Slider* m_strengthSlider;
    float m_strength;
    CCTextInputNode* m_intervalInput;
    Slider* m_intervalSlider;
    float m_interval;

};
