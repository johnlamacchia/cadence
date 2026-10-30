#pragma once

class GJBaseGameLayer;
class cocos2d;

class TeleportPortalObject : public RingObject {
public:
    virtual ~TeleportPortalObject();
    virtual void setPosition(cocos2d::CCPoint const& position);
    virtual void setRotation(float rotation);
    virtual void setStartPos(cocos2d::CCPoint position);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual int addToGroup(int id);
    virtual void removeFromGroup(int id);
    virtual void setRotation2(float rotation);
    virtual void addToGroup2(int id);
    virtual void removeFromGroup2(int id);

    static TeleportPortalObject* create(char const* frame, bool trigger);

    TeleportPortalObject();
    float getTeleportXOff(cocos2d::CCNode* parent);
    bool init(char const* frame, bool trigger);
    void setPositionOverride(cocos2d::CCPoint position);
    void setStartPosOverride(cocos2d::CCPoint position);

    TeleportPortalObject* m_orangePortal;
    bool m_isYellowPortal;
    float m_teleportYOffset;
    bool m_teleportEase;
    bool m_staticForceEnabled;
    float m_staticForce;
    bool m_redirectForceEnabled;
    float m_redirectForceMod;
    float m_redirectForceMin;
    float m_redirectForceMax;
    bool m_saveOffset;
    bool m_ignoreX;
    bool m_ignoreY;
    int m_gravityMode;
    bool m_staticForceAdditive;
    bool m_instantCamera;
    bool m_snapGround;
    bool m_redirectDash;
    cocos2d::CCPoint m_teleportPosition;

};
