#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class EditTriggersPopup : public SetupTriggerPopup {
public:
    virtual ~EditTriggersPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);

    static EditTriggersPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
