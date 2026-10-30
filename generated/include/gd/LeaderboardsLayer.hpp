#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class GJListLayer;
class LoadingCircle;
class TextArea;
class cocos2d;

class LeaderboardsLayer : public cocos2d::CCLayer, public LeaderboardManagerDelegate, public FLAlertLayerProtocol {
public:
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void updateUserScoreFinished();
    virtual void updateUserScoreFailed();
    virtual void loadLeaderboardFinished(cocos2d::CCArray* scores, char const* key);
    virtual void loadLeaderboardFailed(char const* key);

    static LeaderboardsLayer* create(LeaderboardType type, LeaderboardStat stat);
    static cocos2d::CCScene* scene(LeaderboardType type, LeaderboardStat stat);

    LeaderboardsLayer();
    ~LeaderboardsLayer();
    bool init(LeaderboardType type, LeaderboardStat stat);
    bool isCorrect(std::string key);
    void onBack(cocos2d::CCObject* sender);
    void onCreators(cocos2d::CCObject* sender);
    void onGlobal(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onStat(cocos2d::CCObject* sender);
    void onTop(cocos2d::CCObject* sender);
    void onWeek(cocos2d::CCObject* sender);
    void refreshTabs();
    void selectLeaderboard(LeaderboardType type, LeaderboardStat stat);
    void setupLevelBrowser(cocos2d::CCArray* scores);
    void setupTabs();
    void toggleTabButtons();

    GJListLayer* m_list;
    cocos2d::CCArray* m_userScores;
    LeaderboardType m_type;
    LeaderboardStat m_stat;
    CCMenuItemToggler* m_topBtn;
    CCMenuItemToggler* m_globalBtn;
    CCMenuItemToggler* m_creatorsBtn;
    CCMenuItemToggler* m_friendsBtn;
    LoadingCircle* m_circle;
    TextArea* m_noInternet;
    cocos2d::CCArray* m_tabs;
    cocos2d::CCArray* m_modeButtons;

};
