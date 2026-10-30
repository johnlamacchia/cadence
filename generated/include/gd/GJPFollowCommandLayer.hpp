#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class GJPFollowCommandLayer : public SetupTriggerPopup {
public:
    virtual ~GJPFollowCommandLayer();
    virtual void valueDidChange(int property, float value);

    static GJPFollowCommandLayer* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
