#pragma once

class TeleportPortalObject;
class cocos2d;

class SetupTeleportPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTeleportPopup();
    virtual void determineStartValues();
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);

    static SetupTeleportPopup* create(TeleportPortalObject* object, cocos2d::CCArray* objects, int id, bool platformer);

    bool init(TeleportPortalObject* object, cocos2d::CCArray* objects, int id, bool platformer);
    void onTeleportGravity(cocos2d::CCObject* sender);
    void updateTeleportGravityState(int state);

    cocos2d::CCArray* m_teleportGravityToggles;

};
