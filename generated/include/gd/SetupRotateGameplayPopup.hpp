#pragma once

class GameObject;
class RotateGameplayGameObject;
class cocos2d;

class SetupRotateGameplayPopup : public SetupTriggerPopup {
public:
    virtual ~SetupRotateGameplayPopup();
    virtual void onPlusButton(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);

    static SetupRotateGameplayPopup* create(RotateGameplayGameObject* object, cocos2d::CCArray* objects);

    SetupRotateGameplayPopup();
    bool init(RotateGameplayGameObject* object, cocos2d::CCArray* objects);

};
