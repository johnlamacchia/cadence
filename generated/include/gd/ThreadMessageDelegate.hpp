#pragma once

class cocos2d;

class ThreadMessageDelegate {
public:
    virtual void threadListFinished(cocos2d::CCArray*, int, int, int);
    virtual void threadListFailed();
    virtual void threadMessagesFinished(int, cocos2d::CCArray*, bool);
    virtual void threadMessagesFailed(int);
    virtual void threadSearchFinished(int, cocos2d::CCArray*, bool);
    virtual void threadSearchFailed(int);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void threadMembersFinished(int);
    virtual void threadMembersFailed(int);
    virtual void threadInvitesFinished(cocos2d::CCArray*);
    virtual void threadInvitesFailed();

};
