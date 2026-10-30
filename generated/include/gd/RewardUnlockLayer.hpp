#pragma once

class CCMenuItemSpriteExtra;
class CurrencyRewardLayer;
class GJChestSprite;
class GJRewardItem;
class RewardsPage;
class cocos2d;

class RewardUnlockLayer : public FLAlertLayer, public CurrencyRewardDelegate {
public:
    virtual ~RewardUnlockLayer();
    virtual void keyBackClicked();
    virtual void currencyWillExit(CurrencyRewardLayer* layer);

    static RewardUnlockLayer* create(int type, RewardsPage* page);

    void connectionTimeout();
    bool init(int type, RewardsPage* page);
    void labelEnterFinishedO(cocos2d::CCObject* object);
    void onClose(cocos2d::CCObject* sender);
    void playDropSound();
    void playLabelEffect(int count, int type, cocos2d::CCSprite* sprite, cocos2d::CCPoint position, float delay);
    void playRewardEffect();
    bool readyToCollect(GJRewardItem* item);
    void showCloseButton();
    bool showCollectReward(GJRewardItem* item);
    void step2();
    void step3();

    cocos2d::CCArray* m_backgroundObjects;
    RewardsPage* m_rewardsPage;
    GJChestSprite* m_chestSprite;
    int m_chestType;
    bool m_rewardCollected;
    bool m_animationPlayed;
    GJRewardItem* m_rewardItem;
    cocos2d::CCLabelBMFont* m_wrongLabel;
    CCMenuItemSpriteExtra* m_closeBtn;
    CCMenuItemSpriteExtra* m_rewardBtn;
    float m_unkFloat;

};
