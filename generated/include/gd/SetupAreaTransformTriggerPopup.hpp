#pragma once

class EnterEffectObject;
class cocos2d;

class SetupAreaTransformTriggerPopup : public SetupAreaMoveTriggerPopup {
public:
    virtual ~SetupAreaTransformTriggerPopup();

    static SetupAreaTransformTriggerPopup* create(EnterEffectObject* object, cocos2d::CCArray* objects);

    SetupAreaTransformTriggerPopup();
    bool init(EnterEffectObject* object, cocos2d::CCArray* objects);

};
