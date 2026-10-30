#pragma once

class AccountReviewCard;
class AdminAbuseMeta;
class AdminReportedMeta;
class cocos2d;

class CommentAdminDelegate {
public:
    virtual void adminDeleteCommentsFinished(int);
    virtual void adminDeleteCommentsFailed();
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
    virtual void adminUserHistoryFinished(int, cocos2d::CCArray*, int, int, int);
    virtual void adminUserHistoryFailed(int);
    virtual void adminReportedContentFinished(int, cocos2d::CCArray*, AdminReportedMeta const&);
    virtual void adminReportedContentFailed(int);
    virtual void adminClearReportsFinished(int, int);
    virtual void adminClearReportsFailed(int);
    virtual void adminReportQueueFinished(cocos2d::CCArray*, int, int, int);
    virtual void adminReportQueueFailed();
    virtual void adminAbuseInfoFinished(int, cocos2d::CCArray*, AdminAbuseMeta const&);
    virtual void adminAbuseInfoFailed(int);
    virtual void adminAccountReviewFinished(int, AccountReviewCard const&);
    virtual void adminAccountReviewFailed(int);
    virtual void adminCloseCaseFinished(int, int, bool);
    virtual void adminCloseCaseFailed(int);

};
