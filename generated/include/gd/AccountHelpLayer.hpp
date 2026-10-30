#pragma once

class CCMenuItemSpriteExtra;
class FLAlertLayer;
class TextArea;
class cocos2d;

class AccountHelpLayer : public GJDropDownLayer, public GJAccountDelegate, public FLAlertLayerProtocol {
public:
    virtual void customSetup();
    virtual void layerHidden();
    virtual void accountStatusChanged();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static AccountHelpLayer* create();

    AccountHelpLayer();
    ~AccountHelpLayer();
    void doUnlink();
    void exitLayer();
    void onAccountManagement(cocos2d::CCObject* sender);
    void onReLogin(cocos2d::CCObject* sender);
    void onUnlink(cocos2d::CCObject* sender);
    void updatePage();
    void verifyUnlink();

    cocos2d::CCLabelBMFont* m_loginStatusLabel;
    TextArea* m_textArea;
    CCMenuItemSpriteExtra* m_refreshLoginButton;
    CCMenuItemSpriteExtra* m_unlinkAccountButton;
    bool m_unk290;

};
