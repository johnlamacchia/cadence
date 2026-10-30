#pragma once

class EnterEffectObject;
class cocos2d;

class SetupAreaMoveTriggerPopup : public SetupTriggerPopup {
public:
    virtual void updateInputValue(int tag, float& value);
    virtual void updateDefaultTriggerValues();
    virtual void updateInputNode(int tag, float value);
    virtual void valueDidChange(int tag, float value);
    virtual float triggerValueFromSliderValue(int tag, float value);
    virtual float triggerSliderValueFromValue(int tag, float value);

    static SetupAreaMoveTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaMoveTriggerPopup();
    ~SetupAreaMoveTriggerPopup();
    void addAreaDefaultControls(int objectID);
    void getModeValues(int mode, int& direction, bool& inbound, bool& twoDirections);
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);
    void onMode(cocos2d::CCObject* sender);
    void onSpecialTarget(cocos2d::CCObject* sender);
    void updateEnterTargetIDState();

    cocos2d::CCArray* m_modeButtons;
    cocos2d::CCArray* m_targetButtons;

};
