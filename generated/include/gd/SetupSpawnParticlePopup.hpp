#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupSpawnParticlePopup : public SetupTriggerPopup {
public:
    virtual ~SetupSpawnParticlePopup();

    static SetupSpawnParticlePopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
