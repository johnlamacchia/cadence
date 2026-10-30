#pragma once

class AccountReviewCard;
class AdminAbuseMeta;
class AdminReportedMeta;
class Dropdown;
class FLAlertLayer;
class GJUserScore;
class UploadActionPopup;
class cocos2d;

class AccountReviewPopup : public PagedListPopup, public CommentAdminDelegate, public CommentUploadDelegate, public UploadPopupDelegate, public FLAlertLayerProtocol, public DropdownDelegate, public AdminActionConfirmDelegate, public ReviewChoiceDelegate, public ThreadMessageDelegate {
public:
    virtual ~AccountReviewPopup();
    virtual void rebuildContent();
    virtual int requestPage(int);
    virtual void /* TodoReturn */ clearPageContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void dropdownSelectionChanged(Dropdown*, int);
    virtual void reviewChoicePicked(int, int, std::string const&);
    virtual void adminActionConfirmed(int, std::vector<int> const&, std::string const&);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void commentDeleteFailed(int, int);
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void adminAccountReviewFinished(int, AccountReviewCard const&);
    virtual void adminAccountReviewFailed(int);
    virtual void adminReportedContentFinished(int, cocos2d::CCArray*, AdminReportedMeta const&);
    virtual void adminReportedContentFailed(int);
    virtual void adminAbuseInfoFinished(int, cocos2d::CCArray*, AdminAbuseMeta const&);
    virtual void adminAbuseInfoFailed(int);
    virtual void adminUserHistoryFinished(int, cocos2d::CCArray*, int, int, int);
    virtual void adminUserHistoryFailed(int);
    virtual void adminClearReportsFinished(int, int);
    virtual void adminClearReportsFailed(int);
    virtual void adminDeleteCommentsFinished(int);
    virtual void adminDeleteCommentsFailed();
    virtual void adminCloseCaseFinished(int, int, bool);
    virtual void adminCloseCaseFailed(int);

    static AccountReviewPopup* create(int, std::string const&, GJUserScore*);

    void /* TodoReturn */ applySelected(int, bool);
    void /* TodoReturn */ buildDirectReportRow(cocos2d::CCNode*, float);
    void /* TodoReturn */ buildFilterRows(cocos2d::CCArray*);
    void /* TodoReturn */ buildHistoryRows(cocos2d::CCArray*);
    void /* TodoReturn */ buildReportedRows(cocos2d::CCArray*);
    void /* TodoReturn */ clearCard();
    void /* TodoReturn */ clearSelectedReports();
    void /* TodoReturn */ closeAfterDone();
    void /* TodoReturn */ confirmDeleteSelected();
    void /* TodoReturn */ deleteEntry(int);
    void /* TodoReturn */ deleteFailedTick(float);
    void /* TodoReturn */ deleteFilterEntry(int);
    bool init(int, std::string const&, GJUserScore*);
    void /* TodoReturn */ jumpTo(int, int);
    void onBans(cocos2d::CCObject* sender);
    void onCommentHistory(cocos2d::CCObject* sender);
    void onCopyAccountID(cocos2d::CCObject* sender);
    void onDeleteFilterRow(cocos2d::CCObject* sender);
    void onDeleteMenu(cocos2d::CCObject* sender);
    void onDeleteRow(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void onDone(cocos2d::CCObject* sender);
    void onHistoryJump(cocos2d::CCObject* sender);
    void onJumpFilterRow(cocos2d::CCObject* sender);
    void onJumpRow(cocos2d::CCObject* sender);
    void onModActionTaken(cocos2d::CCObject* sender);
    void onNotes(cocos2d::CCObject* sender);
    void onOpenProfile(cocos2d::CCObject* sender);
    void onSelectAll(cocos2d::CCObject* sender);
    void onTab(cocos2d::CCObject* sender);
    void onToggleHistory(cocos2d::CCObject* sender);
    void onToggleRow(cocos2d::CCObject* sender);
    void /* TodoReturn */ openUserComments(bool);
    void /* TodoReturn */ rebuildRows();
    void /* TodoReturn */ refreshCard();
    void /* TodoReturn */ selectedDeletable();
    void /* TodoReturn */ selectTab(int);
    void /* TodoReturn */ threadDeleteAnswered();
    void /* TodoReturn */ updateCard(AccountReviewCard const&);
    void /* TodoReturn */ updateSelectedLabel();
    void /* TodoReturn */ updateTabControls();
    void /* TodoReturn */ viewedByLabel(std::string const&, std::string const&, float);

};
