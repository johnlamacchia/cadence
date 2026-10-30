#pragma once

class CCTextInputNode;
class LoadingCircle;
class cocos2d;

class AccountLoginLayer : public FLAlertLayer, public GJAccountLoginDelegate, public FLAlertLayerProtocol {
public:
    virtual ~AccountLoginLayer();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void loginAccountFinished(int accountID, int userID);
    virtual void loginAccountFailed(AccountError errorType);

    static AccountLoginLayer* create(std::string username);

    cocos2d::extension::CCScale9Sprite* createTextBackground(cocos2d::CCPoint position, char const* text, cocos2d::CCSize size);
    CCTextInputNode* createTextInput(cocos2d::CCPoint position, cocos2d::CCSize size, char const* text, int tag);
    cocos2d::CCLabelBMFont* createTextLabel(cocos2d::CCPoint position, char const* text, cocos2d::CCSize size);
    void disableNodes();
    void hideLoadingUI();
    bool init(std::string username);
    void onClose(cocos2d::CCObject* sender);
    void onForgotPassword(cocos2d::CCObject* sender);
    void onForgotUser(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void resetLabel(int tag);
    void resetLabels();
    void showLoadingUI();
    void toggleUI(bool enable);
    void updateLabel(AccountError type);

    CCTextInputNode* m_usernameInput;
    CCTextInputNode* m_passwordInput;
    cocos2d::CCLabelBMFont* m_usernameLabel;
    cocos2d::CCLabelBMFont* m_passwordLabel;
    LoadingCircle* m_loadingCircle;
    std::string m_username;
    std::string m_password;

};
