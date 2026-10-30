#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupGravityTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupGravityTriggerPopup();
    virtual void valueDidChange(int tag, float value);

    static SetupGravityTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
