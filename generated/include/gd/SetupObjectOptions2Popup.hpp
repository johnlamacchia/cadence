#pragma once

class GameObject;
class cocos2d;

class SetupObjectOptions2Popup : public SetupTriggerPopup {
public:
    virtual ~SetupObjectOptions2Popup();
    virtual void onPlusButton(cocos2d::CCObject* sender);

    static SetupObjectOptions2Popup* create(GameObject* object, cocos2d::CCArray* objects);

    SetupObjectOptions2Popup();
    bool init(GameObject* object, cocos2d::CCArray* objects);

};
