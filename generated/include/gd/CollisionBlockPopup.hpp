#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class CollisionBlockPopup : public SetupTriggerPopup {
public:
    virtual ~CollisionBlockPopup();
    virtual void valueDidChange(int, float);

    static CollisionBlockPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
