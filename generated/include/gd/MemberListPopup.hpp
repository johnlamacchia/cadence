#pragma once

class FLAlertLayer;
class GJMessageThread;
class GJUserScore;
class ThreadViewPopup;
class cocos2d;

class MemberListPopup : public PagedListPopup, public ThreadMessageDelegate, public FriendPickerDelegate, public FLAlertLayerProtocol {
public:
    virtual ~MemberListPopup();
    virtual int requestPage(int);
    virtual void friendPicked(GJUserScore*);
    virtual bool isRecipientSelected(int);
    virtual void /* TodoReturn */ selectedRecipientCount();
    virtual void /* TodoReturn */ selectedRecipients();
    virtual void /* TodoReturn */ pickerRowState(int);
    virtual void /* TodoReturn */ pickerCountBase();
    virtual void friendPickerAdd();
    virtual void friendPickerClosed();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void threadMembersFinished(int);
    virtual void threadMembersFailed(int);
    virtual void threadMessagesFinished(int, cocos2d::CCArray*, bool);
    virtual void threadMessagesFailed(int);
    virtual void threadSearchFinished(int, cocos2d::CCArray*, bool);
    virtual void threadSearchFailed(int);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();

    static MemberListPopup* create(GJMessageThread*, ThreadViewPopup*, bool);

    void /* TodoReturn */ applyAddOutcome(GJMessageThread*);
    void /* TodoReturn */ buildRows();
    bool init(GJMessageThread*, ThreadViewPopup*, bool);
    void onAddMember(cocos2d::CCObject* sender);
    void onAddPolicy(cocos2d::CCObject* sender);
    void onKickMember(cocos2d::CCObject* sender);
    void onMemberProfile(cocos2d::CCObject* sender);
    void onRevokeInvite(cocos2d::CCObject* sender);
    void onTransferOwner(cocos2d::CCObject* sender);
    void /* TodoReturn */ opPending();
    void /* TodoReturn */ refreshHost();
    void refreshPolicyBtn();

};
