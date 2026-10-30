#pragma once

class CameraTriggerGameObject;
class GameObject;
class cocos2d;

class SetupStaticCameraPopup : public SetupTriggerPopup {
public:
    virtual ~SetupStaticCameraPopup();
    virtual void valueDidChange(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupStaticCameraPopup* create(CameraTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(CameraTriggerGameObject* object, cocos2d::CCArray* objects);
    void updateState();

};
