#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class DialogLayer;
class FLAlertLayer;
class ListButtonBar;
class SimplePlayer;
class cocos2d;

class GJGarageLayer : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol, public GameRateDelegate, public ListButtonBarDelegate, public DialogDelegate, public CharacterColorDelegate, public RewardedVideoDelegate {
public:
    virtual bool init();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void listButtonBarSwitchedPage(ListButtonBar* bar, int page);
    virtual void showUnlockPopup(int id, UnlockType type);
    virtual void updateRate();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void dialogClosed(DialogLayer* layer);
    virtual void playerColorChanged();
    virtual void rewardedVideoFinished();

    static std::string achievementForUnlock(int id, UnlockType type);
    static std::string descriptionForUnlock(int id, UnlockType type);
    static GJGarageLayer* node();
    static cocos2d::CCScene* scene();
    static std::string titleForUnlock(int id, UnlockType type);

    GJGarageLayer();
    ~GJGarageLayer();
    cocos2d::CCArray* getItems(IconType type);
    cocos2d::CCArray* getItems(int count, int page, IconType type, int current);
    std::string getLockFrame(int id, UnlockType type);
    void onArrow(cocos2d::CCObject* sender);
    void onBack(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNavigate(cocos2d::CCObject* sender);
    void onPaint(cocos2d::CCObject* sender);
    void onRewardedVideo(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);
    void onSelectTab(cocos2d::CCObject* sender);
    void onShards(cocos2d::CCObject* sender);
    void onShop(cocos2d::CCObject* sender);
    void onSpecial(cocos2d::CCObject* sender);
    void onToggleItem(cocos2d::CCObject* sender);
    void playRainbowEffect();
    void playShadowEffect();
    void selectTab(IconType type);
    void setupIconSelect();
    void setupPage(int page, IconType type);
    void setupSpecialPage();
    void showUnlockPopupNew(int id, UnlockType type);
    void toggleGlow();
    void updatePlayerColors();
    void updatePlayerName(char const* name);

    CCTextInputNode* m_usernameInput;
    SimplePlayer* m_playerObject;
    cocos2d::CCArray* m_tabButtons;
    cocos2d::CCArray* m_pageButtons;
    cocos2d::CCNode* m_unknown;
    cocos2d::CCSprite* m_supporter;
    bool m_hasClosed;
    IconType m_iconType;
    std::map<IconType, int> m_iconPages;
    cocos2d::CCSprite* m_cursor1;
    cocos2d::CCSprite* m_cursor2;
    CCMenuItemSpriteExtra* m_currentIcon;
    ListButtonBar* m_iconSelection;
    CCMenuItemSpriteExtra* m_leftArrow;
    CCMenuItemSpriteExtra* m_rightArrow;
    cocos2d::CCMenu* m_navDotMenu;
    int m_iconID;
    IconType m_selectedIconType;
    bool m_videoPlaying;

};
