#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupRotateCommandPopup : public SetupTriggerPopup {
public:
    virtual ~SetupRotateCommandPopup();
    virtual void updateInputNode(int tag, float value);
    virtual void valueDidChange(int tag, float value);
    virtual float triggerValueFromSliderValue(int tag, float value);
    virtual float triggerSliderValueFromValue(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupRotateCommandPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onInfiniteDuration(cocos2d::CCObject* sender);
    void updateControlVisibility();

};
