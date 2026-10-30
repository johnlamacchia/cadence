#pragma once

class EnhancedGameObject;
class GameObject;
class cocos2d;

class SetupRotatePopup : public SetupTriggerPopup {
public:
    virtual ~SetupRotatePopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupRotatePopup* create(EnhancedGameObject* object, cocos2d::CCArray* objects);

    bool init(EnhancedGameObject* object, cocos2d::CCArray* objects);

};
