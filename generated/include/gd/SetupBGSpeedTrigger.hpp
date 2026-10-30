#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupBGSpeedTrigger : public SetupTriggerPopup {
public:
    virtual ~SetupBGSpeedTrigger();

    static SetupBGSpeedTrigger* create(EffectGameObject* object, cocos2d::CCArray* objects, int id);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects, int id);

};
