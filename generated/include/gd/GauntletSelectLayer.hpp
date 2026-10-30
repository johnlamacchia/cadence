#pragma once

class BoomScrollLayer;
class CCMenuItemSpriteExtra;
class LoadingCircle;
class TextArea;
class cocos2d;

class GauntletSelectLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate, public LevelManagerDelegate, public RewardedVideoDelegate {
public:
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void rewardedVideoFinished();
    virtual void scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page);
    virtual void scrollLayerScrolledToPage(BoomScrollLayer* layer, int page);
    virtual void loadLevelsFinished(cocos2d::CCArray* gauntlets, char const* key, int type);
    virtual void loadLevelsFailed(char const* key, int type);

    static GauntletSelectLayer* create(int unused);
    static cocos2d::CCScene* scene(int unused);

    GauntletSelectLayer();
    ~GauntletSelectLayer();
    void goToPage(int page, bool instant);
    bool init(int unused);
    void onBack(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void onRefresh(cocos2d::CCObject* sender);
    void playUnlockAnimation();
    void setupGauntlets();
    void showUnlockGauntlet();
    void unblockPlay();
    void unlockAnimationFinished();
    void unlockAnimationStep2();
    void unlockAnimationStep3();
    void updateArrows();

    cocos2d::CCSprite* m_backgroundSprite;
    BoomScrollLayer* m_scrollLayer;
    CCMenuItemSpriteExtra* m_leftButton;
    CCMenuItemSpriteExtra* m_rightButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    bool m_exiting;
    bool m_playing;
    TextArea* m_tryAgainText;
    LoadingCircle* m_loadingCircle;
    cocos2d::CCDictionary* m_gauntlets;
    bool m_playBlocked;
    int m_gauntletID;
    bool m_videoPlaying;
    bool m_locked;

};
