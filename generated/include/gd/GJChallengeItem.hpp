#pragma once

class DS_Dictionary;

class GJChallengeItem : public cocos2d::CCObject {
public:
    virtual ~GJChallengeItem();
    virtual void encodeWithCoder(DS_Dictionary* dsdict);
    virtual bool canEncode();

    static GJChallengeItem* create();
    static GJChallengeItem* create(GJChallengeType challengeType, int goal, int reward, int timeLeft, std::string questName);
    static GJChallengeItem* createFromString(std::string string);
    static GJChallengeItem* createWithCoder(DS_Dictionary* dsdict);

    void dataLoaded(DS_Dictionary* dsdict);
    void incrementCount(int add);
    bool init(GJChallengeType challengeType, int goal, int reward, int timeLeft, std::string questName);
    void setCount(int value);

    GJChallengeType m_challengeType;
    geode::SeedValueRSV m_count;
    geode::SeedValueRSV m_reward;
    geode::SeedValueRSV m_goal;
    int m_timeLeft;
    bool m_canClaim;
    int m_position;
    std::string m_name;

};
