#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupCountTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupCountTriggerPopup();
    virtual void valueDidChange(int, float);

    static SetupCountTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
