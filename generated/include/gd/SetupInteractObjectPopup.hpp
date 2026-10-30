#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupInteractObjectPopup : public SetupTriggerPopup {
public:
    virtual ~SetupInteractObjectPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);

    static SetupInteractObjectPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateItems();

};
