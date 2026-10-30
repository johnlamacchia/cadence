#pragma once

class CCMenuItemSpriteExtra;
class RewardUnlockLayer;
class cocos2d;

class RewardsPage : public FLAlertLayer, public FLAlertLayerProtocol, public GJRewardDelegate {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void rewardsStatusFinished(int type);
    virtual void rewardsStatusFailed();

    static RewardsPage* create();
    static char const* getRewardFrame(int type, int state);

    RewardsPage();
    ~RewardsPage();
    void onClose(cocos2d::CCObject* sender);
    void onFreeStuff(cocos2d::CCObject* sender);
    void onReward(cocos2d::CCObject* sender);
    void tryGetRewards();
    void unlockLayerClosed(RewardUnlockLayer* layer);
    void updateTimers(float dt);

    cocos2d::CCLabelBMFont* m_leftLabel;
    cocos2d::CCLabelBMFont* m_rightLabel;
    CCMenuItemSpriteExtra* m_leftChest;
    CCMenuItemSpriteExtra* m_rightChest;
    bool m_leftOpen;
    bool m_rightOpen;
    RewardUnlockLayer* m_openLayer;

};
