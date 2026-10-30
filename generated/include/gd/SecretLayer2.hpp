#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class DialogLayer;
class FLAlertLayer;
class SecretLayer;
class cocos2d;

class SecretLayer2 : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol, public DialogDelegate {
public:
    virtual bool init();
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void dialogClosed(DialogLayer* layer);

    static SecretLayer2* create();
    static cocos2d::CCScene* scene();

    SecretLayer2();
    ~SecretLayer2();
    std::string getBasicMessage();
    std::string getErrorMessage();
    std::string getMessage();
    std::string getThreadMessage();
    cocos2d::CCNode* nodeWithTag(int tag);
    void onBack(cocos2d::CCObject* sender);
    void onDoor(cocos2d::CCObject* sender);
    void onSecretLevel(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void playCoinEffect();
    void selectAThread();
    void showCompletedLevel();
    void showSecretLevel();
    void updateMessageLabel(std::string text);
    void updateSearchLabel(char const* text);

    int m_threadTag;
    int m_basicMessageIndex;
    int m_selectedThread;
    int m_threadMessageIndex;
    int m_basicMessageCount;
    int m_errorMessageIndex;
    CCTextInputNode* m_searchInput;
    cocos2d::CCLabelBMFont* m_messageLabel;
    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCDictionary* m_messageThreads;
    bool m_exiting;
    int m_doorDialogIndex;
    CCMenuItemSpriteExtra* m_secretLevelButton;
    cocos2d::CCArray* m_secretLevelLabels;
    CCMenuItemSpriteExtra* m_secretDoorButton;

};
