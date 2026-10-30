#pragma once

class EnterEffectObject;
class cocos2d;

class SetupAreaFadeTriggerPopup : public SetupAreaMoveTriggerPopup {
public:
    virtual ~SetupAreaFadeTriggerPopup();

    static SetupAreaFadeTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaFadeTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);

};
