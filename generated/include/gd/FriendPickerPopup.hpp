#pragma once

class CCTextInputNode;
class FriendPickerDelegate;
class cocos2d;

class FriendPickerPopup : public PagedListPopup, public VirtualRowSource, public UserListDelegate {
public:
    virtual ~FriendPickerPopup();
    virtual int requestPage(int);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual int virtualRowCount();
    virtual int virtualRowHeight(int);
    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*);
    virtual void getUserListFinished(cocos2d::CCArray*, UserListType);
    virtual void getUserListFailed(UserListType, GJErrorCode);
    virtual void textChanged(CCTextInputNode*);

    static FriendPickerPopup* create(FriendPickerDelegate*, bool, bool, bool, bool);

    void /* TodoReturn */ buildRows();
    void /* TodoReturn */ delayedRebuild(float);
    bool init(FriendPickerDelegate*, bool, bool, bool, bool);
    void onClearAll(cocos2d::CCObject* sender);
    void onDone(cocos2d::CCObject* sender);
    void onGlobalSearch(cocos2d::CCObject* sender);
    void onPickFriend(cocos2d::CCObject* sender);
    void onShowProfile(cocos2d::CCObject* sender);
    void onToggleSelectedView(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateCountLabel();

};
