#pragma once

class DialogLayer;
class cocos2d;

class CreatorLayer : public cocos2d::CCLayer, public cocos2d::CCSceneTransitionDelegate, public DialogDelegate, public GJContentStatusDelegate {
public:
    virtual ~CreatorLayer();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void sceneWillResume();
    virtual void dialogClosed(DialogLayer* layer);
    virtual void contentStatusFinished();

    static CreatorLayer* create();
    static cocos2d::CCScene* scene();

    CreatorLayer();
    bool canPlayOnlineLevels();
    void checkQuestsStatus();
    void /* TodoReturn */ checkTimedStatus();
    void onAdventureMap(cocos2d::CCObject* sender);
    void onBack(cocos2d::CCObject* sender);
    void onChallenge(cocos2d::CCObject* sender);
    void onDailyLevel(cocos2d::CCObject* sender);
    void onEventLevel(cocos2d::CCObject* sender);
    void onFameLevels(cocos2d::CCObject* sender);
    void onFeaturedLevels(cocos2d::CCObject* sender);
    void onGauntlets(cocos2d::CCObject* sender);
    void onLeaderboards(cocos2d::CCObject* sender);
    void onMapPacks(cocos2d::CCObject* sender);
    void onMultiplayer(cocos2d::CCObject* sender);
    void onMyLevels(cocos2d::CCObject* sender);
    void onOnlineLevels(cocos2d::CCObject* sender);
    void onOnlyFullVersion(cocos2d::CCObject* sender);
    void onPaths(cocos2d::CCObject* sender);
    void onSavedLevels(cocos2d::CCObject* sender);
    void onSecretVault(cocos2d::CCObject* sender);
    void onTopLists(cocos2d::CCObject* sender);
    void onTreasureRoom(cocos2d::CCObject* sender);
    void onWeeklyLevel(cocos2d::CCObject* sender);

    cocos2d::CCSprite* m_secretDoorSprite;
    cocos2d::CCSprite* m_questsSprite;
    cocos2d::CCSprite* m_dailySprite;
    cocos2d::CCSprite* m_weeklySprite;
    cocos2d::CCSprite* m_eventSprite;
    cocos2d::CCSprite* m_gauntletSprite;
    int m_vaultDialogIndex;
    int m_versusDialogIndex;

};
