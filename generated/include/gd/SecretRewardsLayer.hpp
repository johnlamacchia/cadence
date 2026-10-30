#pragma once

class BoomScrollLayer;
class CCMenuItemSpriteExtra;
class DialogLayer;
class cocos2d;

class SecretRewardsLayer : public cocos2d::CCLayer, public DialogDelegate, public BoomScrollLayerDelegate {
public:
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void dialogClosed(DialogLayer* layer);
    virtual void scrollLayerMoved(cocos2d::CCPoint position);

    static SecretRewardsLayer* create(bool fromShop);
    static cocos2d::CCScene* scene(bool fromShop);

    SecretRewardsLayer();
    ~SecretRewardsLayer();
    void createSecondaryLayer(int chestType);
    void fadeInMusic();
    void fadeInOutMusic();
    cocos2d::CCArray* generateChestItems(int chestType);
    cocos2d::ccColor3B getPageColor(int page);
    void goToPage(int page);
    bool init(bool fromShop);
    void moveToMainLayer(cocos2d::CCObject* sender);
    void moveToSecondaryLayer(int page);
    void onBack(cocos2d::CCObject* sender);
    void onChestType(cocos2d::CCObject* sender);
    void onGoldChest(cocos2d::CCObject* sender);
    void onSelectItem(cocos2d::CCObject* sender);
    void onShop(cocos2d::CCObject* sender);
    void onSpecialItem(cocos2d::CCObject* sender);
    void onSwitchPage(cocos2d::CCObject* sender);
    void showDialog01();
    void showDialog03();
    void showDialogDiamond();
    void showDialogMechanic();
    void showLockedChest();
    void showShop(int shop);
    void switchToOpenedState(CCMenuItemSpriteExtra* button);
    void updateBackButton();
    void updateUnlockedGoldLabel();
    void updateUnlockedLabel();

    cocos2d::CCLayer* m_mainLayer;
    cocos2d::CCLayer* m_secondaryLayer;
    cocos2d::CCLabelBMFont* m_chestLabel;
    cocos2d::CCLabelBMFont* m_chestCounter;
    cocos2d::CCSprite* m_goldPriceSprite;
    cocos2d::CCDictionary* m_chestCounters;
    cocos2d::CCLabelBMFont* m_keysLabel;
    cocos2d::CCLabelBMFont* m_goldKeysLabel;
    cocos2d::CCSprite* m_keySprite;
    cocos2d::CCSprite* m_goldKeySprite;
    void* m_unknown;
    CCMenuItemSpriteExtra* m_leftButton;
    CCMenuItemSpriteExtra* m_rightButton;
    cocos2d::CCSprite* m_backSprite;
    int m_unkSize4_1;
    int m_unkSize4_2;
    BoomScrollLayer* m_mainScrollLayer;
    BoomScrollLayer* m_secondaryScrollLayer;
    cocos2d::CCSprite* m_backgroundSprite;
    int m_scratchDialogIndex;
    int m_potborDialogIndex;
    int m_diamondDialogIndex;
    int m_mechanicDialogIndex;
    bool m_inMainLayer;
    GJRewardType m_rewardType;
    int m_lockedDialogIndex;

};
