#pragma once

class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupTimeWarpPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTimeWarpPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupTimeWarpPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateTimeWarp();
    void updateTimeWarpLabel();

    cocos2d::CCLabelBMFont* m_timeWarpLabel;
    Slider* m_timeWarpSlider;
    float m_timeWarpMod;

};
