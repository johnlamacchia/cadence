#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupKeyframeAnimPopup : public SetupTriggerPopup {
public:
    virtual ~SetupKeyframeAnimPopup();

    static SetupKeyframeAnimPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
