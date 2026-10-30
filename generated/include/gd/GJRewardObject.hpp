#pragma once

class DS_Dictionary;

class GJRewardObject : public cocos2d::CCObject {
public:
    virtual ~GJRewardObject();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();

    static GJRewardObject* create();
    static GJRewardObject* create(SpecialRewardItem type, int total, int itemID);
    static GJRewardObject* createItemUnlock(UnlockType type, int id);
    static GJRewardObject* createWithCoder(DS_Dictionary* dict);

    void dataLoaded(DS_Dictionary* dict);
    bool init(SpecialRewardItem specialRewardItem, int total, int itemID);
    bool isSpecialType();

    SpecialRewardItem m_specialRewardItem;
    UnlockType m_unlockType;
    int m_itemID;
    int m_total;

};
