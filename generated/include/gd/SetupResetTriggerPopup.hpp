#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupResetTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupResetTriggerPopup();

    static SetupResetTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
