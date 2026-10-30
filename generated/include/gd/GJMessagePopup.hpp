#pragma once

class CCMenuItemSpriteExtra;
class GJUserMessage;
class LoadingCircle;
class UploadActionPopup;
class cocos2d;

class GJMessagePopup : public FLAlertLayer, public UploadActionDelegate, public UploadPopupDelegate, public FLAlertLayerProtocol, public DownloadMessageDelegate {
public:
    virtual ~GJMessagePopup();
    virtual void keyBackClicked();
    virtual void downloadMessageFinished(GJUserMessage* message);
    virtual void downloadMessageFailed(int id);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static GJMessagePopup* create(GJUserMessage* message);

    void blockUser();
    bool init(GJUserMessage* message);
    void loadFromGJMessage(GJUserMessage* message);
    void onBlock(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onNewThread(cocos2d::CCObject* sender);
    void onRemove(cocos2d::CCObject* sender);
    void onReport(cocos2d::CCObject* sender);

    bool m_actionSuccess;
    cocos2d::CCLabelBMFont* m_errorLabel;
    GJUserMessage* m_message;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_closeButton;
    UploadActionPopup* m_uploadPopup;

};
