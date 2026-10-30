#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class EnterEffectObject;
class HSVWidgetPopup;
class cocos2d;

class SetupEnterEffectPopup : public SetupTriggerPopup, public HSVWidgetDelegate {
public:
    virtual ~SetupEnterEffectPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void updateInputValue(int tag, float& value);
    virtual void updateInputNode(int tag, float value);
    virtual void valueDidChange(int tag, float value);
    virtual float triggerValueFromSliderValue(int tag, float value);
    virtual float triggerSliderValueFromValue(int tag, float value);
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value);

    static SetupEnterEffectPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects, int id);

    SetupEnterEffectPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects, int id);
    void onEnterType(cocos2d::CCObject* sender);
    void onHSV(cocos2d::CCObject* sender);
    void onNextFreeEnterEffectID(cocos2d::CCObject* sender);
    void updateHSVButton();

    int m_objectID;
    CCMenuItemToggler* m_enterOnlyToggler;
    CCMenuItemToggler* m_exitOnlyToggler;
    int m_enterType;
    CCMenuItemSpriteExtra* m_hsvButton;
    cocos2d::ccHSVValue m_hsvValue;
    bool m_hsvChanged;

};
