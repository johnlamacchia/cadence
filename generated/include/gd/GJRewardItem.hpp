#pragma once

class DS_Dictionary;
class GJRewardObject;
class cocos2d;

class GJRewardItem : public cocos2d::CCObject {
public:
    virtual ~GJRewardItem();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();

    static GJRewardItem* create();
    static GJRewardItem* create(int chestID, int timeRemaining, std::string rewardStr);
    static GJRewardItem* createSpecial(GJRewardType rewardType, int orbs, int diamonds, SpecialRewardItem specialType1, int specialCount1, SpecialRewardItem specialType2, int specialCount2, int unlockType, int itemID);
    static GJRewardItem* createWithCoder(DS_Dictionary* dict);
    static GJRewardItem* createWithObject(GJRewardType type, GJRewardObject* object);
    static GJRewardItem* createWithObjects(GJRewardType type, cocos2d::CCArray* objects);
    static SpecialRewardItem getNextShardType(SpecialRewardItem type);
    static SpecialRewardItem getRandomNonMaxShardType();
    static SpecialRewardItem getRandomShardType();
    static bool isShardType(SpecialRewardItem type);
    static std::string rewardItemToStat(SpecialRewardItem rewardItem);

    void dataLoaded(DS_Dictionary* dict);
    int getRewardCount(SpecialRewardItem type);
    GJRewardObject* getRewardObjectForType(SpecialRewardItem type);
    bool init(int chestID, int timeRemaining, std::string rewardStr);

    int m_chestID;
    int m_timeRemaining;
    GJRewardType m_rewardType;
    cocos2d::CCArray* m_rewardObjects;
    bool m_unk;

};
