#pragma once

class CCMenuItemSpriteExtra;
class CCSpriteGrayscale;
class CCSpriteWithHue;
class CCTextInputNode;
class DialogLayer;
class FLAlertLayer;
class LoadingCircle;
class LoadingCircleSprite;
class SecretLayer;
class TextArea;
class cocos2d;

class SecretLayer5 : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol, public DialogDelegate, public GJOnlineRewardDelegate {
public:
    virtual bool init();
    virtual void onExit();
    virtual void updateTweenActionInt(float value, int tag);
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void dialogClosed(DialogLayer* layer);
    virtual void onlineRewardStatusFinished(std::string key);
    virtual void onlineRewardStatusFailed();

    static SecretLayer5* create();
    static cocos2d::CCScene* scene();

    SecretLayer5();
    ~SecretLayer5();
    void animateHead();
    void claimOnlineReward();
    void fadeInMessage();
    void fadeInSubmitMessage();
    void fadeOutMessage();
    void finishLoadingState();
    std::string getMessage();
    void handleSecretResponse();
    void hideTextInput();
    void incrementChatIdx();
    cocos2d::CCNode* nodeWithTag(int tag);
    void onBack(cocos2d::CCObject* sender);
    void onSubmit(cocos2d::CCObject* sender);
    void playWinSFX();
    void showDialog(int index);
    void showFailAnimation();
    void showFirstDialog();
    void showSuccessAnimation();
    void showTextInput();
    void unlockUI();
    void updateMessageLabel(std::string text);
    void updateSearchLabel(char const* text);

    int m_threadTag;
    int m_basicMessageIndex;
    int m_selectedThread;
    int m_threadMessageIndex;
    int m_basicMessageCount;
    int m_errorMessageIndex;
    CCTextInputNode* m_textInput;
    cocos2d::CCLabelBMFont* m_messageLabel;
    CCMenuItemSpriteExtra* m_wraithButton;
    bool m_exiting;
    void* m_unk1f0;
    cocos2d::CCArray* m_torchFires;
    bool m_loading;
    int m_rewardStatus;
    std::string m_chestID;
    bool m_uiLocked;
    CCSpriteWithHue* m_wraithSprite;
    CCSpriteGrayscale* m_wraithGraySprite;
    cocos2d::CCSprite* m_eyesSprite;
    cocos2d::CCSprite* m_background;
    cocos2d::CCSprite* m_lockSprite;
    std::vector<int> m_soundEffects;
    int m_chatIndex;
    cocos2d::extension::CCScale9Sprite* m_inputBackground;
    LoadingCircleSprite* m_circleSprite;
    TextArea* m_messageArea;

};
