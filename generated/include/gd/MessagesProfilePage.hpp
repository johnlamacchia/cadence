#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class GJComment;
class GJCommentListLayer;
class LoadingCircle;
class ProfilePage;
class UploadActionPopup;
class cocos2d;

class MessagesProfilePage : public FLAlertLayer, public FLAlertLayerProtocol, public UploadActionDelegate, public UploadPopupDelegate, public MessageListDelegate {
public:
    virtual ~MessagesProfilePage();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void loadMessagesFinished(cocos2d::CCArray* messages, char const* key);
    virtual void loadMessagesFailed(char const* key, GJErrorCode errorType);
    virtual void forceReloadMessages(bool sent);
    virtual void setupPageInfo(std::string info, char const* key);
    virtual bool ownsTouchPrio();

    static MessagesProfilePage* create(bool sent);

    void deleteSelected();
    bool init(bool sent);
    bool isCorrect(char const* key);
    void loadPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onSelectionChanged(cocos2d::CCObject* sender);
    void onSentMessages(cocos2d::CCObject* sender);
    void onToggleAllObjects(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void /* TodoReturn */ selectedCount();
    void setupCommentsBrowser(cocos2d::CCArray* messages);
    void untoggleAll();
    void updateLevelsLabel();
    void updatePageArrows();
    void /* TodoReturn */ updateSelectUI();

    bool m_sentMessages;
    std::string m_messageKey;
    cocos2d::CCLabelBMFont* m_levelsLabel;
    cocos2d::CCLabelBMFont* m_errorLabel;
    GJCommentListLayer* m_listLayer;
    LoadingCircle* m_loadingCircle;
    UploadActionPopup* m_actionPopup;
    void* m_unkPtr;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    CCMenuItemToggler* m_toggleAllToggler;
    int m_itemCount;
    int m_pageStartIdx;
    int m_pageEndIdx;
    int m_page;
    bool m_toggledAll;

};
