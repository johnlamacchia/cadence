#pragma once

class CCMenuItemSpriteExtra;
class FLAlertLayer;
class LoadingCircle;
class TextArea;
class cocos2d;

class AccountLayer : public GJDropDownLayer, public GJAccountDelegate, public GJAccountBackupDelegate, public GJAccountSyncDelegate, public FLAlertLayerProtocol {
public:
    virtual ~AccountLayer();
    virtual void customSetup();
    virtual void layerHidden();
    virtual void backupAccountFinished();
    virtual void backupAccountFailed(BackupAccountError errorType, int response);
    virtual void syncAccountFinished();
    virtual void syncAccountFailed(BackupAccountError errorType, int response);
    virtual void accountStatusChanged();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static AccountLayer* create();

    void createToggleButton(std::string text, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    void doBackup();
    void doSync();
    void exitLayer();
    void hideLoadingUI();
    void onBackup(cocos2d::CCObject* sender);
    void onHelp(cocos2d::CCObject* sender);
    void onLogin(cocos2d::CCObject* sender);
    void onMore(cocos2d::CCObject* sender);
    void onRegister(cocos2d::CCObject* sender);
    void onSync(cocos2d::CCObject* sender);
    void showLoadingUI();
    void toggleUI(bool enable);
    void updatePage(bool changed);

    cocos2d::CCLabelBMFont* m_linkedAccountTitle;
    TextArea* m_textArea;
    CCMenuItemSpriteExtra* m_loginButton;
    CCMenuItemSpriteExtra* m_registerButton;
    CCMenuItemSpriteExtra* m_backupButton;
    CCMenuItemSpriteExtra* m_syncButton;
    CCMenuItemSpriteExtra* m_helpButton;
    CCMenuItemSpriteExtra* m_moreButton;
    LoadingCircle* m_loadingCircle;
    int m_accountHelpRelated;
    bool m_isLoggedIn;

};
