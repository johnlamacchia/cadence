#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GJComment;
class GJCommentListLayer;
class GJSelectionStrip;
class LoadingCircle;
class ProfilePage;
class UploadActionPopup;
class cocos2d;

class FRequestProfilePage : public FLAlertLayer, public FLAlertLayerProtocol, public UploadActionDelegate, public UploadPopupDelegate, public FriendRequestDelegate, public UserListDelegate {
public:
    virtual ~FRequestProfilePage();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void loadFRequestsFinished(cocos2d::CCArray* scores, char const* key);
    virtual void loadFRequestsFailed(char const* key, GJErrorCode errorType);
    virtual void setupPageInfo(std::string info, char const* key);
    virtual void forceReloadRequests(bool sent);
    virtual bool ownsTouchPrio();
    virtual void getUserListFinished(cocos2d::CCArray*, UserListType);
    virtual void getUserListFailed(UserListType, GJErrorCode);
    virtual void textChanged(CCTextInputNode*);

    static FRequestProfilePage* create(bool sent);

    void deleteSelected();
    void /* TodoReturn */ doSearch(float);
    void /* TodoReturn */ exitGlobalMode();
    bool init(bool sent);
    bool isCorrect(char const* key);
    void loadPage(int page);
    void onClearSearch(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void onGlobalSearch(cocos2d::CCObject* sender);
    void onMarkRead(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onSelectionChanged(cocos2d::CCObject* sender);
    void onSelectMode(cocos2d::CCObject* sender);
    void onToggleAllObjects(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void /* TodoReturn */ removeFromNewCount(int);
    void /* TodoReturn */ selectedCount();
    void setupCommentsBrowser(cocos2d::CCArray* scores);
    void untoggleAll();
    void updateLevelsLabel();
    void /* TodoReturn */ updateModeButtons();
    void updatePageArrows();
    void /* TodoReturn */ updateSelectUI();

    bool m_sent;
    bool m_ignored;
    std::string m_key;
    std::string m_query;
    cocos2d::CCLabelBMFont* m_pageLabel;
    cocos2d::CCLabelBMFont* m_errorLabel;
    cocos2d::CCLabelBMFont* m_wrongLabel;
    GJCommentListLayer* m_listLayer;
    LoadingCircle* m_circle;
    UploadActionPopup* m_uploadPopup;
    void* m_unk2e8;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    cocos2d::CCNode* m_inboxNode;
    cocos2d::CCNode* m_sentNode;
    cocos2d::CCNode* m_ignoredNode;
    CCTextInputNode* m_searchInput;
    cocos2d::CCArray* m_userScores;
    bool m_triedToLoadUsers;
    GJSelectionStrip* m_selectionStrip;
    CCMenuItemSpriteExtra* m_markReadButton;
    int m_itemCount;
    int m_pageStartIdx;
    int m_pageEndIdx;
    int m_page;

};
