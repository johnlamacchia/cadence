#pragma once

class cocos2d;

class RetryLevelLayer : public GJDropDownLayer, public RewardedVideoDelegate {
public:
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void customSetup();
    virtual void showLayer(bool instant);
    virtual void enterAnimFinished();
    virtual void rewardedVideoFinished();
    virtual bool shouldOffsetRewardCurrency();
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);

    static RetryLevelLayer* create();

    RetryLevelLayer();
    ~RetryLevelLayer();
    const char* getEndText();
    void onEveryplay(cocos2d::CCObject* sender);
    void onFullVersion(cocos2d::CCObject* sender);
    void onMenu(cocos2d::CCObject* sender);
    void onReplay(cocos2d::CCObject* sender);
    void onRewardedVideo(cocos2d::CCObject* sender);
    void setupLastProgress();

    cocos2d::CCMenu* m_mainMenu;
    bool m_exitingMenu;
    bool m_unk1e9;

};
