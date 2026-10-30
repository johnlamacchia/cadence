#pragma once

class CCMenuItemSpriteExtra;
class LoadingCircle;
class TextArea;
class UploadPopup;
class UploadPopupDelegate;
class cocos2d;

class UploadActionPopup : public FLAlertLayer {
public:
    virtual ~UploadActionPopup();
    virtual void keyBackClicked();

    static UploadActionPopup* create(UploadPopupDelegate* delegate, std::string str);

    void closePopup();
    bool init(UploadPopupDelegate* delegate, std::string str);
    void onClose(cocos2d::CCObject* sender);
    void showFailMessage(std::string message);
    void /* TodoReturn */ showFailMessageOrDismiss(std::string);
    void showSuccessMessage(std::string message);

    UploadPopupDelegate* m_delegate;
    TextArea* m_textArea;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_closeButton;
    bool m_succeeded;

};
