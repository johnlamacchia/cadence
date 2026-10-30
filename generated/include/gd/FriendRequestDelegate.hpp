#pragma once

class cocos2d;

class FriendRequestDelegate {
public:
    virtual void loadFRequestsFinished(cocos2d::CCArray* scores, char const* key);
    virtual void loadFRequestsFailed(char const* key, GJErrorCode errorType);
    virtual void setupPageInfo(std::string info, char const* key);
    virtual void forceReloadRequests(bool sent);

};
