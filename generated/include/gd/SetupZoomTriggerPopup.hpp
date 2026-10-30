#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupZoomTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupZoomTriggerPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupZoomTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);

};
