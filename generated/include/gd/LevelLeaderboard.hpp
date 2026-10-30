#pragma once

class CCMenuItemSpriteExtra;
class GJComment;
class GJCommentListLayer;
class GJGameLevel;
class LoadingCircle;
class TextArea;
class cocos2d;

class LevelLeaderboard : public FLAlertLayer, public LeaderboardManagerDelegate, public FLAlertLayerProtocol {
public:
    virtual ~LevelLeaderboard();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void loadLeaderboardFinished(cocos2d::CCArray* scores, char const* key);
    virtual void loadLeaderboardFailed(char const* key);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void updateUserScoreFinished();
    virtual void updateUserScoreFailed();
    virtual bool ownsTouchPrio();

    static LevelLeaderboard* create(GJGameLevel* level, LevelLeaderboardType type, LevelLeaderboardMode mode);

    void deleteLocalScores();
    cocos2d::CCArray* getLocalScores();
    CCMenuItemSpriteExtra* getSpriteButton(std::string frame, cocos2d::SEL_MenuHandler selector, cocos2d::CCMenu* menu, float scale, cocos2d::CCPoint position, int button);
    bool init(GJGameLevel* level, LevelLeaderboardType type, LevelLeaderboardMode mode);
    bool isCorrect(char const* key);
    void loadScores();
    void onChangeMode(cocos2d::CCObject* sender);
    void onChangeType(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onDeleteLocalScores(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void reloadLeaderboard(LevelLeaderboardType type, LevelLeaderboardMode mode);
    void setupLeaderboard(cocos2d::CCArray* scores);

    GJGameLevel* m_level;
    LevelLeaderboardType m_type;
    LevelLeaderboardMode m_mode;
    cocos2d::CCArray* m_scores;
    GJCommentListLayer* m_list;
    LoadingCircle* m_circle;
    TextArea* m_noInternet;
    CCMenuItemSpriteExtra* m_refreshBtn;
    CCMenuItemSpriteExtra* m_timeBtn;
    CCMenuItemSpriteExtra* m_pointsBtn;

};
