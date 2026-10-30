#pragma once

class cocos2d;

class UserListDelegate {
public:
    virtual void getUserListFinished(cocos2d::CCArray* scores, UserListType type);
    virtual void getUserListFailed(UserListType type, GJErrorCode errorType);
    virtual void userListChanged(cocos2d::CCArray* scores, UserListType type);
    virtual void forceReloadList(UserListType type);

};
