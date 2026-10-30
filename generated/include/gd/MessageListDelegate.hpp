#pragma once

class cocos2d;

class MessageListDelegate {
public:
    virtual void loadMessagesFinished(cocos2d::CCArray* messages, char const* key);
    virtual void loadMessagesFailed(char const* key, GJErrorCode errorType);
    virtual void forceReloadMessages(bool sent);
    virtual void setupPageInfo(std::string info, char const* key);

};
