#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupStopTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupStopTriggerPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);

    static SetupStopTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
