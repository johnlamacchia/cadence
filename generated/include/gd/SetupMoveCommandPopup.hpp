#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupMoveCommandPopup : public SetupTriggerPopup {
public:
    virtual ~SetupMoveCommandPopup();
    virtual void updateInputValue(int tag, float& value);
    virtual void updateInputNode(int tag, float value);
    virtual void valueDidChange(int tag, float value);
    virtual float triggerValueFromSliderValue(int tag, float value);
    virtual float triggerSliderValueFromValue(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupMoveCommandPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateControlVisibility();

    bool m_smallStep;

};
