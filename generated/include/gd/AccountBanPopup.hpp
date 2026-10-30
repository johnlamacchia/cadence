#pragma once

class Dropdown;
class UploadActionPopup;
class cocos2d;

class AccountBanPopup : public SetupTriggerPopup, public CommentAdminDelegate, public UploadPopupDelegate, public DropdownDelegate, public AdminActionConfirmDelegate {
public:
    virtual ~AccountBanPopup();
    virtual void keyBackClicked();
    virtual void pageChanged();
    virtual void onClose(cocos2d::CCObject*);
    virtual void adminActionConfirmed(int, std::vector<int> const&, std::string const&);
    virtual void adminCommentBanFinished();
    virtual void adminCommentBanFailed();
    virtual void adminCommentUnbanFinished();
    virtual void adminCommentUnbanFailed();
    virtual void adminUploadBanFinished();
    virtual void adminUploadBanFailed();
    virtual void adminUploadUnbanFinished();
    virtual void adminUploadUnbanFailed();
    virtual void adminMessageBanFinished();
    virtual void adminMessageBanFailed();
    virtual void adminMessageUnbanFinished();
    virtual void adminMessageUnbanFailed();
    virtual void adminFreezeFinished();
    virtual void adminFreezeFailed();
    virtual void adminUnfreezeFinished();
    virtual void adminUnfreezeFailed();
    virtual void adminTrendingBanFinished();
    virtual void adminTrendingBanFailed();
    virtual void adminTrendingUnbanFinished();
    virtual void adminTrendingUnbanFailed();
    virtual void adminNameLockFinished();
    virtual void adminNameLockFailed();
    virtual void adminNameUnlockFinished();
    virtual void adminNameUnlockFailed();
    virtual void adminQueueMuteFinished();
    virtual void adminQueueMuteFailed();
    virtual void adminQueueUnmuteFinished();
    virtual void adminQueueUnmuteFailed();
    virtual void onClosePopup(UploadActionPopup*);
    virtual void dropdownSelectionChanged(Dropdown*, int);

    static AccountBanPopup* create(int, std::string const&, int, int, int, int, int, int);

    void /* TodoReturn */ actionFailed();
    void /* TodoReturn */ banLanded(std::string const&);
    void /* TodoReturn */ banStateShort(int);
    void /* TodoReturn */ banStateText(int);
    void /* TodoReturn */ executeAction(int, std::vector<int> const&, std::string const&);
    bool init(int, std::string const&, int, int, int, int, int, int);
    void onCommentBan(cocos2d::CCObject* sender);
    void onCommentUnban(cocos2d::CCObject* sender);
    void onFreeze(cocos2d::CCObject* sender);
    void onLevelUploadBan(cocos2d::CCObject* sender);
    void onLevelUploadUnban(cocos2d::CCObject* sender);
    void onMessageBan(cocos2d::CCObject* sender);
    void onMessageUnban(cocos2d::CCObject* sender);
    void onNameLock(cocos2d::CCObject* sender);
    void onNameUnlock(cocos2d::CCObject* sender);
    void onQueueMute(cocos2d::CCObject* sender);
    void onQueueUnmute(cocos2d::CCObject* sender);
    void onTrendingBan(cocos2d::CCObject* sender);
    void onTrendingUnban(cocos2d::CCObject* sender);
    void onUnfreeze(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshBanRows();

};
