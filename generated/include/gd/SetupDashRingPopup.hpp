#pragma once

class DashRingObject;
class RingObject;
class cocos2d;

class SetupDashRingPopup : public SetupTriggerPopup {
public:
    virtual ~SetupDashRingPopup();

    static SetupDashRingPopup* create(DashRingObject* object, cocos2d::CCArray* objects);

    SetupDashRingPopup();
    bool init(DashRingObject* object, cocos2d::CCArray* objects);

};
