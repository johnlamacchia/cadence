#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupCollisionStateTriggerPopup : public SetupInstantCollisionTriggerPopup {
public:
    virtual ~SetupCollisionStateTriggerPopup();

    static SetupCollisionStateTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    SetupCollisionStateTriggerPopup();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
