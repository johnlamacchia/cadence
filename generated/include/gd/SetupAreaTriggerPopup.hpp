#pragma once

class EnterEffectObject;
class cocos2d;

class SetupAreaTriggerPopup : public SetupAreaMoveTriggerPopup {
public:
    virtual ~SetupAreaTriggerPopup();

    static SetupAreaTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);

};
