#pragma once

class CommentUploadDelegate {
public:
    virtual void commentUploadFinished(int parentID);
    virtual void commentUploadFailed(int parentID, CommentError errorType);
    virtual void commentDeleteFailed(int id, int parentID);
    virtual void bulkCommentDeleteFinished(int);
    virtual void bulkCommentDeleteFailed();

};
