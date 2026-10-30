#pragma once

class DS_Dictionary;
class cocos2d;

class AchievementManager : public cocos2d::CCNode {
public:
    virtual ~AchievementManager();
    virtual bool init();

    static AchievementManager* sharedState();

    std::string achievementForUnlock(int id, UnlockType type);
    void addAchievement(std::string identifier, std::string title, std::string achievedDescription, std::string unachievedDescription, std::string icon, int limits);
    void addManualAchievements();
    bool areAchievementsEarned(cocos2d::CCArray* achievements);
    void checkAchFromUnlock(char const* id);
    void dataLoaded(DS_Dictionary* dict);
    void encodeDataTo(DS_Dictionary* dict);
    void firstSetup();
    cocos2d::CCDictionary* getAchievementRewardDict();
    cocos2d::CCDictionary* getAchievementsWithID(char const* id);
    cocos2d::CCArray* getAllAchievements();
    cocos2d::CCArray* getAllAchievementsSorted(bool available);
    bool isAchievementAvailable(std::string id);
    bool isAchievementEarned(char const* ach);
    int limitForAchievement(std::string id);
    void notifyAchievement(char const* title, char const* description, char const* icon);
    void notifyAchievementWithID(char const* id);
    int percentageForCount(int count, int total);
    int percentForAchievement(char const* id);
    void reportAchievementWithID(char const* id, int percent, bool dontNotify);
    void reportPlatformAchievementWithID(char const* id, int percent);
    void resetAchievement(char const* id);
    void resetAchievements();
    void setup();
    void storeAchievementUnlocks();

    cocos2d::CCArray* m_allAchievements;
    cocos2d::CCDictionary* m_platformAchievements;
    cocos2d::CCDictionary* m_achievementUnlocks;
    void* m_unkPtrUnused;
    cocos2d::CCDictionary* m_reportedAchievements;
    bool m_dontNotify;
    cocos2d::CCArray* m_allAchievementsSorted;
    int m_order;
    cocos2d::CCDictionary* m_unAchieved;

};
