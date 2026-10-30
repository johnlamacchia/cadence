#pragma once

class ShareCommentLayer;

class ShareCommentDelegate {
public:
    virtual void shareCommentClosed(std::string text, ShareCommentLayer* layer);

};
