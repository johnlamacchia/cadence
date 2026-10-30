#pragma once

class NotificationsPage;
class cocos2d;

class NotificationFilterPopup : public SetupTriggerPopup {
public:
    virtual ~NotificationFilterPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);

    static NotificationFilterPopup* create(NotificationsPage*);

    bool init(NotificationsPage*);
    void onDownloadMilestones(cocos2d::CCObject* sender);
    void onFollowerMilestones(cocos2d::CCObject* sender);
    void onFollowUploads(cocos2d::CCObject* sender);
    void onFriendAccepts(cocos2d::CCObject* sender);
    void onFriendRated(cocos2d::CCObject* sender);
    void onFriendUploads(cocos2d::CCObject* sender);
    void onSocialAlerts(cocos2d::CCObject* sender);
    void /* TodoReturn */ savedMask();
    void /* TodoReturn */ toggleSource(int);

};
