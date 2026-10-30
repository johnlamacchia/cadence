#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class FLAlertLayer;
class GJComment;
class GJCommentListLayer;
class LoadingCircle;
class ProfilePage;
class UploadActionPopup;
class cocos2d;

class FriendsProfilePage : public PagedListPopup, public VirtualRowSource, public FLAlertLayerProtocol, public UploadActionDelegate, public UploadPopupDelegate, public UserListDelegate {
public:
    virtual ~FriendsProfilePage();
    virtual void getUserListFinished(cocos2d::CCArray* scores, UserListType type);
    virtual void getUserListFailed(UserListType type, GJErrorCode errorType);
    virtual void userListChanged(cocos2d::CCArray* scores, UserListType type);
    virtual void forceReloadList(UserListType type);
    virtual int requestPage(int);
    virtual int virtualRowCount();
    virtual int virtualRowHeight(int);
    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void textChanged(CCTextInputNode*);

    static FriendsProfilePage* create(UserListType type);

    void /* TodoReturn */ filterUsersWithQuery(std::string const&);
    void /* TodoReturn */ getActiveUserList();
    bool init(UserListType type);
    void onClearSearch(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onGlobalSearch(cocos2d::CCObject* sender);
    void onSendMessage(cocos2d::CCObject* sender);
    void onSwitchList(cocos2d::CCObject* sender);
    void onToggleSort(cocos2d::CCObject* sender);
    void onToggleSortDir(cocos2d::CCObject* sender);
    void onUnblock(cocos2d::CCObject* sender);
    void onUnfollow(cocos2d::CCObject* sender);
    void onUnmute(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void /* TodoReturn */ rebuildList();
    void /* TodoReturn */ sortUsers();
    void /* TodoReturn */ updateCountLabel();
    void /* TodoReturn */ updateSortLabel();

    UserListType m_type;
    cocos2d::CCLabelBMFont* m_noInternet;
    cocos2d::CCLabelBMFont* m_totalFriends;
    GJCommentListLayer* m_listLayer;
    LoadingCircle* m_circle;
    void* m_unk532;
    cocos2d::CCArray* m_users;
    CCMenuItemSpriteExtra* m_refreshBtn;

};
