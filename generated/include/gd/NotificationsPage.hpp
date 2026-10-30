#pragma once

class cocos2d;

class NotificationsPage : public FLAlertLayer, public NotificationListDelegate {
public:
    virtual ~NotificationsPage();
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual bool ownsTouchPrio();
    virtual void loadNotificationsFinished(cocos2d::CCArray*, int, int, int);
    virtual void loadNotificationsFailed(int, GJErrorCode);
    virtual void notificationRewardFinished(int);
    virtual void notificationRewardFailed(int);
    virtual void notificationMuteFinished(int);
    virtual void notificationMuteFailed(int);

    static NotificationsPage* create();

    void /* TodoReturn */ loadPage(int);
    void onClose(cocos2d::CCObject* sender);
    void onFilter(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void /* TodoReturn */ reloadCells();
    void /* TodoReturn */ setupBrowser(cocos2d::CCArray*);
    void /* TodoReturn */ updateCountLabel();
    void /* TodoReturn */ updatePageArrows();

};
