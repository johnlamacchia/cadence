#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class GJFollowCommandLayer : public SetupTriggerPopup {
public:
    virtual ~GJFollowCommandLayer();
    virtual void valueDidChange(int, float);

    static GJFollowCommandLayer* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
