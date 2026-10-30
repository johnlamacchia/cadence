#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class FLAlertLayer;
class cocos2d;

class SecretLayer : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol {
public:
    virtual bool init();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static SecretLayer* create();
    static cocos2d::CCScene* scene();

    SecretLayer();
    ~SecretLayer();
    std::string getBasicMessage();
    std::string getMessage();
    std::string getThreadMessage();
    cocos2d::CCNode* nodeWithTag(int tag);
    void onBack(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void playCoinEffect();
    void selectAThread();
    void updateMessageLabel(std::string text);
    void updateSearchLabel(char const* text);

    int m_basicMessageIndex;
    int m_selectedThread;
    int m_threadMessageIndex;
    int m_basicMessageCount;
    int m_doomedIndex;
    CCTextInputNode* m_searchInput;
    cocos2d::CCLabelBMFont* m_messageLabel;
    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCDictionary* m_messageThreads;
    int m_threadTag;

};
