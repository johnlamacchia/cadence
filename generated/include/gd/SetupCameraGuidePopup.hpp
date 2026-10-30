#pragma once

class CameraTriggerGameObject;
class GameObject;
class cocos2d;

class SetupCameraGuidePopup : public SetupTriggerPopup {
public:
    virtual ~SetupCameraGuidePopup();

    static SetupCameraGuidePopup* create(CameraTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(CameraTriggerGameObject* object, cocos2d::CCArray* objects);

};
