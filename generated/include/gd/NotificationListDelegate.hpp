#pragma once

class cocos2d;

class NotificationListDelegate {
public:
    virtual void loadNotificationsFinished(cocos2d::CCArray*, int, int, int);
    virtual void loadNotificationsFailed(int, GJErrorCode);
    virtual void notificationRewardFinished(int);
    virtual void notificationRewardFailed(int);
    virtual void notificationMuteFinished(int);
    virtual void notificationMuteFailed(int);

};
