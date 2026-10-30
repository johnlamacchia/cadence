#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupPickupTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupPickupTriggerPopup();
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupPickupTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateState();

};
