#pragma once

class CCTextInputNode;
class UploadActionPopup;
class cocos2d;

class GJWriteMessagePopup : public FLAlertLayer, public UploadMessageDelegate, public UploadPopupDelegate, public FLAlertLayerProtocol {
public:
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void uploadMessageFinished(int accountID);
    virtual void uploadMessageFailed(int accountID);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static GJWriteMessagePopup* create(int accountID, int messageID);

    void closeMessagePopup(bool fullExit);
    bool init(int accountID, int messageID);
    void onClearBody(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onSend(cocos2d::CCObject* sender);
    void updateBody(std::string body);
    void updateCharCountLabel(int type);
    void updateSubject(std::string subject);
    void updateText(std::string text, int type);

    int m_unk298;
    int m_messageID;
    int m_accountID;
    CCTextInputNode* m_messageInput;
    CCTextInputNode* m_subjectInput;
    std::string m_messageText;
    std::string m_subjectText;
    cocos2d::CCLabelBMFont* m_messageCountLabel;
    cocos2d::CCLabelBMFont* m_subjectCountLabel;
    UploadActionPopup* m_uploadPopup;
    bool m_uploadSuccess;

};
