#pragma once

class CCTextInputNode;
class FLAlertLayer;
class GJMessageThread;
class cocos2d;

class InvitesPopup : public PagedListPopup, public ThreadMessageDelegate, public FLAlertLayerProtocol {
public:
    virtual ~InvitesPopup();
    virtual bool init();
    virtual int requestPage(int);
    virtual void textChanged(CCTextInputNode*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void threadInvitesFinished(cocos2d::CCArray*);
    virtual void threadInvitesFailed();
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);

    static InvitesPopup* create();

    void /* TodoReturn */ acceptInvite(GJMessageThread*);
    void /* TodoReturn */ buildRows();
    void /* TodoReturn */ declineInvite(GJMessageThread*);
    void /* TodoReturn */ layoutTitleInfo();
    void onAccept(cocos2d::CCObject* sender);
    void onClearSearch(cocos2d::CCObject* sender);
    void onDecline(cocos2d::CCObject* sender);
    void onToggleIgnored(cocos2d::CCObject* sender);
    void onViewInviter(cocos2d::CCObject* sender);
    void onViewMembers(cocos2d::CCObject* sender);
    void onViewRequest(cocos2d::CCObject* sender);
    void /* TodoReturn */ removeInviteRow(int);

};
