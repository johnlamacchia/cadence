#pragma once

class CCTextInputNode;
class LoadingCircle;
class cocos2d;

class AccountRegisterLayer : public FLAlertLayer, public GJAccountRegisterDelegate, public FLAlertLayerProtocol {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual bool allowTextInput(CCTextInputNode* node);
    virtual void registerAccountFinished();
    virtual void registerAccountFailed(AccountError errorType);

    static AccountRegisterLayer* create();

    AccountRegisterLayer();
    ~AccountRegisterLayer();
    cocos2d::extension::CCScale9Sprite* createTextBackground(cocos2d::CCPoint position, cocos2d::CCSize size);
    CCTextInputNode* createTextInput(cocos2d::CCPoint position, cocos2d::CCSize size, std::string text, int tag);
    cocos2d::CCLabelBMFont* createTextLabel(cocos2d::CCPoint position, std::string text, cocos2d::CCSize size);
    void disableNodes();
    void hideLoadingUI();
    void onClose(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void resetLabel(int tag);
    void resetLabels();
    void showLoadingUI();
    void toggleUI(bool enable);
    void updateLabel(AccountError type);
    void /* TodoReturn */ updatePasswordInfo(std::string);
    bool validEmail(std::string email);
    bool validUser(std::string username);

    CCTextInputNode* m_usernameField;
    CCTextInputNode* m_passwordField;
    CCTextInputNode* m_confirmPasswordField;
    CCTextInputNode* m_emailField;
    CCTextInputNode* m_verifyEmailField;
    cocos2d::CCLabelBMFont* m_usernameLabel;
    cocos2d::CCLabelBMFont* m_passwordLabel;
    cocos2d::CCLabelBMFont* m_passwordInfoLabel;
    cocos2d::CCLabelBMFont* m_confirmPasswordLabel;
    cocos2d::CCLabelBMFont* m_emailLabel;
    cocos2d::CCLabelBMFont* m_verifyEmailLabel;
    LoadingCircle* m_loadingCircle;
    bool m_lockInput;

};
