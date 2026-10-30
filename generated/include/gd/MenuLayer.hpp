#pragma once

class CCMenuItemSpriteExtra;
class FLAlertLayer;
class MenuGameLayer;
class cocos2d;

class MenuLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol, public GooglePlayDelegate, public MusicDownloadDelegate {
public:
    virtual ~MenuLayer();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void googlePlaySignedIn();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void musicActionFinished(GJMusicAction);

    static MenuLayer* get();
    static cocos2d::CCScene* scene(bool isVideoOptionsOpen);

    void /* TodoReturn */ checkParentalPending();
    void endGame();
    void firstNetworkTest();
    void onAchievements(cocos2d::CCObject* sender);
    void onCreator(cocos2d::CCObject* sender);
    void onDaily(cocos2d::CCObject* sender);
    void onDiscord(cocos2d::CCObject* sender);
    void onEveryplay(cocos2d::CCObject* sender);
    void onFacebook(cocos2d::CCObject* sender);
    void onFreeLevels(cocos2d::CCObject* sender);
    void onFullVersion(cocos2d::CCObject* sender);
    void onGameCenter(cocos2d::CCObject* sender);
    void onGarage(cocos2d::CCObject* sender);
    void onGooglePlayGames(cocos2d::CCObject* sender);
    void onMoreGames(cocos2d::CCObject* sender);
    void onMyProfile(cocos2d::CCObject* sender);
    void onNewgrounds(cocos2d::CCObject* sender);
    void onNews(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onOptionsInstant();
    void onPlay(cocos2d::CCObject* sender);
    void onQuit(cocos2d::CCObject* sender);
    void onReportQueue(cocos2d::CCObject* sender);
    void onRobTop(cocos2d::CCObject* sender);
    void onStats(cocos2d::CCObject* sender);
    void onTrailer(cocos2d::CCObject* sender);
    void onTwitch(cocos2d::CCObject* sender);
    void onTwitter(cocos2d::CCObject* sender);
    void onYouTube(cocos2d::CCObject* sender);
    void openOptions(bool videoOptions);
    void showGCQuestion();
    void showMeltdownPromo();
    void showTOS();
    void syncPlatformAchievements(float dt);
    void tryShowAd(float dt);
    void updateNewsBadge();
    void updateUserProfileButton();
    void videoOptionsClosed();
    void videoOptionsOpened();
    void willClose();

    bool m_showingTOS;
    cocos2d::CCSprite* m_gpSprite;
    cocos2d::CCSprite* m_viewProfileSprite;
    cocos2d::CCLabelBMFont* m_profileLabel;
    CCMenuItemSpriteExtra* m_profileButton;
    CCMenuItemSpriteExtra* m_reportQueueButton;
    cocos2d::CCSprite* m_characterSelectSprite;
    cocos2d::CCSprite* m_newsButton;
    void* m_unknown;
    MenuGameLayer* m_menuGameLayer;

};
