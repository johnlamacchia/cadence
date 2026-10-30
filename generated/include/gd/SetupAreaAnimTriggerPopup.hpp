#pragma once

class EnterEffectObject;
class Slider;
class cocos2d;

class SetupAreaAnimTriggerPopup : public SetupAreaTintTriggerPopup {
public:
    virtual ~SetupAreaAnimTriggerPopup();
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);

    static SetupAreaAnimTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects, int id);

    SetupAreaAnimTriggerPopup();
    void createValueControlAdvancedAnim(int property, std::string label, cocos2d::CCPoint position, float scale, bool noSlider, InputValueType valueType, int length, bool arrows, float min, float max, int page, int group, GJInputStyle inputStyle);
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects, int id);
    void onDeactivateAnimValue(cocos2d::CCObject* sender);
    void updateTargetIDLabel();

};
