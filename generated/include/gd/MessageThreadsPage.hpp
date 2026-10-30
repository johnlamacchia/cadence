#pragma once

class CCTextInputNode;
class GJUserScore;
class cocos2d;

class MessageThreadsPage : public FLAlertLayer, public ThreadMessageDelegate, public FLAlertLayerProtocol, public FriendPickerDelegate {
public:
    virtual ~MessageThreadsPage();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void show();
    virtual bool ownsTouchPrio();
    virtual void textInputClosed(CCTextInputNode*);
    virtual void friendPicked(GJUserScore*);
    virtual bool isRecipientSelected(int);
    virtual void /* TodoReturn */ selectedRecipientCount();
    virtual void /* TodoReturn */ selectedRecipients();
    virtual void clearRecipients();
    virtual void friendPickerClosed();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void threadListFinished(cocos2d::CCArray*, int, int, int);
    virtual void threadListFailed();
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);

    static MessageThreadsPage* create();

    void applyThreadList(cocos2d::CCArray*, int, int, int, bool);
    void checkActivityAbove(float);
    bool isCovered();
    void loadPage(int, bool);
    void onClearSearch(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void /* TodoReturn */ onInboxPoll(float);
    void onLegacyMessages(cocos2d::CCObject* sender);
    void onMarkRead(cocos2d::CCObject* sender);
    void onNewThread(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onRefresh(cocos2d::CCObject* sender);
    void onScrollToTop(cocos2d::CCObject* sender);
    void onSearch(cocos2d::CCObject* sender);
    void onSelectAll(cocos2d::CCObject* sender);
    void onSelectFilter(cocos2d::CCObject* sender);
    void onSelectionChanged(cocos2d::CCObject* sender);
    void onShowInvites(cocos2d::CCObject* sender);
    void onThreadsChanged(cocos2d::CCObject* sender);
    void onUserFilter(cocos2d::CCObject* sender);
    void /* TodoReturn */ refetchIfQueued();
    void /* TodoReturn */ refreshUserFilterBtn();
    void /* TodoReturn */ selectedCount();
    void setActivityAbove(bool);
    void setInboxPoll(float);
    void /* TodoReturn */ setupThreadBrowser(cocos2d::CCArray*, float);
    void /* TodoReturn */ syncSearchFromBox();
    void /* TodoReturn */ updateFilterButtons();
    void /* TodoReturn */ updateInviteButton();
    void /* TodoReturn */ updatePageArrows(int, int, int);
    void /* TodoReturn */ updateSelectUI();
    void /* TodoReturn */ userFilterCSV();

};
