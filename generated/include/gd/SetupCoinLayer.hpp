#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupCoinLayer : public SetupTriggerPopup {
public:
    virtual ~SetupCoinLayer();

    static SetupCoinLayer* create(EffectGameObject* object, cocos2d::CCArray* objects);

    SetupCoinLayer();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
