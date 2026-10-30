#pragma once

class GameObject;
class UISettingsGameObject;
class cocos2d;

class UIObjectSettingsPopup : public SetupTriggerPopup {
public:
    virtual ~UIObjectSettingsPopup();
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static UIObjectSettingsPopup* create(UISettingsGameObject* object, cocos2d::CCArray* objects);

    bool init(UISettingsGameObject* object, cocos2d::CCArray* objects);

};
