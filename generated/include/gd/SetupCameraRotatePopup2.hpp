#pragma once

class EffectGameObject;
class GameObject;
class SetupCameraRotatePopup;
class cocos2d;

class SetupCameraRotatePopup2 : public SetupTriggerPopup {
public:
    virtual ~SetupCameraRotatePopup2();

    static SetupCameraRotatePopup2* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
