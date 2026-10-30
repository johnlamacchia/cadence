#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class MultiTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~MultiTriggerPopup();

    static MultiTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
