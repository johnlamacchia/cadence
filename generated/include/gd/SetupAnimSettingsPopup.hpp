#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupAnimSettingsPopup : public SetupTriggerPopup {
public:
    virtual ~SetupAnimSettingsPopup();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupAnimSettingsPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void updateAnimSettings();

};
