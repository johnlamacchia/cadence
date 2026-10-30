#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupCollisionTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupCollisionTriggerPopup();
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int, float);

    static SetupCollisionTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateSpecialNodes();

};
