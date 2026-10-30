#pragma once

class CCMenuItemSpriteExtra;
class FLAlertLayer;
class GJNotification;
class cocos2d;

class GJNotificationCell : public TableViewCell, public FLAlertLayerProtocol {
public:
    virtual ~GJNotificationCell();
    virtual bool init();
    virtual void draw();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    GJNotificationCell(char const*, float, float);
    void loadFromNotification(GJNotification*);
    void onClaimReward(cocos2d::CCObject* sender);
    void onMute(cocos2d::CCObject* sender);
    void onShowBody(cocos2d::CCObject* sender);
    void onViewTarget(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshChest();
    void updateBGColor(int);

    GJNotification* m_notification;
    CCMenuItemSpriteExtra* m_claimBtn;

};
