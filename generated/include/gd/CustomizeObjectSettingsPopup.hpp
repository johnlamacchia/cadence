#pragma once

class CustomizeObjectLayer;
class GameObject;
class cocos2d;

class CustomizeObjectSettingsPopup : public SetupTriggerPopup {
public:
    virtual ~CustomizeObjectSettingsPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static CustomizeObjectSettingsPopup* create(GameObject* object, cocos2d::CCArray* objects, CustomizeObjectLayer* layer);

    CustomizeObjectSettingsPopup();
    bool init(GameObject* object, cocos2d::CCArray* objects, CustomizeObjectLayer* layer);

    CustomizeObjectLayer* m_customizeLayer;
    bool m_settingsChanged;

};
