#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupGameplayOffsetPopup : public SetupTriggerPopup {
public:
    virtual ~SetupGameplayOffsetPopup();
    virtual void updateInputValue(int tag, float& value);
    virtual void updateInputNode(int tag, float value);
    virtual float triggerValueFromSliderValue(int tag, float value);
    virtual float triggerSliderValueFromValue(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupGameplayOffsetPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    SetupGameplayOffsetPopup();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onDefaultValues(cocos2d::CCObject* sender);

};
