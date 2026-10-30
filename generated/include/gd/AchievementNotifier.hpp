#pragma once

class AchievementBar;
class cocos2d;

class AchievementNotifier : public cocos2d::CCNode {
public:
    virtual ~AchievementNotifier();
    virtual bool init();

    static AchievementNotifier* sharedState();

    void achievementDisplayFinished();
    void notifyAchievement(char const* title, char const* desc, char const* icon, bool quest);
    void showNextAchievement();
    void willSwitchToScene(cocos2d::CCScene* scene);

    cocos2d::CCScene* m_nextScene;
    cocos2d::CCArray* m_achievementBarArray;
    AchievementBar* m_activeAchievementBar;

};
