#pragma once

class AdvancedFollowEditObject;
class cocos2d;

class SetupAdvFollowEditPhysicsPopup : public SetupTriggerPopup {
public:
    virtual ~SetupAdvFollowEditPhysicsPopup();
    virtual void valueDidChange(int tag, float value);

    static SetupAdvFollowEditPhysicsPopup* create(AdvancedFollowEditObject* object, cocos2d::CCArray* objects);

    bool init(AdvancedFollowEditObject* object, cocos2d::CCArray* objects);

};
