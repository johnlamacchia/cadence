#pragma once

class CCMenuItemSpriteExtra;
class EnterEffectObject;
class HSVWidgetPopup;
class cocos2d;

class SetupAreaTintTriggerPopup : public SetupAreaMoveTriggerPopup, public HSVWidgetDelegate {
public:
    virtual ~SetupAreaTintTriggerPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value);

    static SetupAreaTintTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaTintTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);
    void onHSV(cocos2d::CCObject* sender);
    void updateHSVButton();

    CCMenuItemSpriteExtra* m_hsvButton;
    cocos2d::ccHSVValue m_hsvValue;
    bool m_hsvChanged;

};
