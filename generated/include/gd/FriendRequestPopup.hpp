#pragma once

class CCMenuItemSpriteExtra;
class GJFriendRequest;
class LoadingCircle;
class UploadActionPopup;
class cocos2d;

class FriendRequestPopup : public FLAlertLayer, public UploadActionDelegate, public UploadPopupDelegate, public FLAlertLayerProtocol {
public:
    virtual ~FriendRequestPopup();
    virtual void keyBackClicked();
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static FriendRequestPopup* create(GJFriendRequest*, bool);

    void blockUser();
    bool init(GJFriendRequest*, bool);
    void loadFromGJFriendRequest(GJFriendRequest* request);
    void onAccept(cocos2d::CCObject* sender);
    void onBlock(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onIgnore(cocos2d::CCObject* sender);
    void onRemove(cocos2d::CCObject* sender);
    void onReport(cocos2d::CCObject* sender);

    void* m_unk;
    GJFriendRequest* m_request;
    LoadingCircle* m_circle;
    CCMenuItemSpriteExtra* m_closeBtn;
    UploadActionPopup* m_popup;

};
