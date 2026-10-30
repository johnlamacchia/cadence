#pragma once

class GameObject;
class SFXTriggerGameObject;
class cocos2d;

class SetupSFXEditPopup : public SetupAudioTriggerPopup {
public:
    virtual ~SetupSFXEditPopup();

    static SetupSFXEditPopup* create(SFXTriggerGameObject* object, cocos2d::CCArray* objects, bool songTrigger);

    bool init(SFXTriggerGameObject* object, cocos2d::CCArray* objects, bool songTrigger);

};
