#pragma once

class DailyLevelNode;
class GJGameLevel;
class LevelPage;
class LoadingCircle;
class cocos2d;

class DailyLevelPage : public FLAlertLayer, public FLAlertLayerProtocol, public GJDailyLevelDelegate, public LevelDownloadDelegate {
public:
    virtual ~DailyLevelPage();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void dailyStatusFinished(GJTimedLevelType type);
    virtual void dailyStatusFailed(GJTimedLevelType type, GJErrorCode errorType);
    virtual void levelDownloadFinished(GJGameLevel* level);
    virtual void levelDownloadFailed(int response);

    static DailyLevelPage* create(GJTimedLevelType type);

    void claimLevelReward(DailyLevelNode* node, GJGameLevel* level, cocos2d::CCPoint position);
    void createDailyNode(GJGameLevel* level, bool instant, float delay, bool isNew);
    void createNodeIfLoaded();
    void downloadAndCreateNode();
    void exitDailyNode(DailyLevelNode* node, float delay);
    int getDailyTime();
    std::string getDailyTimeString(int timeLeft);
    bool init(GJTimedLevelType type);
    void onClose(cocos2d::CCObject* sender);
    void onTheSafe(cocos2d::CCObject* sender);
    void refreshDailyPage();
    void skipDailyLevel(DailyLevelNode* node, GJGameLevel* level);
    void tryGetDailyStatus();
    void updateTimers(float dt);

    cocos2d::CCLabelBMFont* m_timeLabel;
    LoadingCircle* m_timeCircle;
    LoadingCircle* m_nodeCircle;
    bool m_gettingDailyStatus;
    DailyLevelNode* m_dailyNode;
    bool m_downloadStarted;
    GJTimedLevelType m_type;
    int m_downloadLevelID;

};
