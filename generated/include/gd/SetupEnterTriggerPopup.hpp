#pragma once

class CCMenuItemToggler;
class EnterEffectObject;
class cocos2d;

class SetupEnterTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupEnterTriggerPopup();
    virtual void determineStartValues();

    static SetupEnterTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupEnterTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);
    void onEnterType(cocos2d::CCObject* sender);

    CCMenuItemToggler* m_enterOnlyToggler;
    CCMenuItemToggler* m_exitOnlyToggler;
    int m_enterType;

};
