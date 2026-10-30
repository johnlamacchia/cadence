#pragma once

class EnterEffectObject;
class cocos2d;

class SetupAreaRotateTriggerPopup : public SetupAreaMoveTriggerPopup {
public:
    virtual ~SetupAreaRotateTriggerPopup();

    static SetupAreaRotateTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaRotateTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);

};
