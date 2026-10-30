#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupInstantCollisionTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupInstantCollisionTriggerPopup();
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);

    static SetupInstantCollisionTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    SetupInstantCollisionTriggerPopup();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateSpecialNodes();

};
