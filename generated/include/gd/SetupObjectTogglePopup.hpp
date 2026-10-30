#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupObjectTogglePopup : public SetupTriggerPopup {
public:
    virtual ~SetupObjectTogglePopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);

    static SetupObjectTogglePopup* create(EffectGameObject* object, cocos2d::CCArray* objects, bool platformer);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects, bool platformer);

};
