#pragma once

class AdvancedFollowEditObject;
class cocos2d;

class SetupAdvFollowRetargetPopup : public SetupTriggerPopup {
public:
    virtual ~SetupAdvFollowRetargetPopup();
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);

    static SetupAdvFollowRetargetPopup* create(AdvancedFollowEditObject* object, cocos2d::CCArray* objects);

    bool init(AdvancedFollowEditObject* object, cocos2d::CCArray* objects);

};
