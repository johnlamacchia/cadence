#pragma once

class GJStoreItem : public cocos2d::CCNode {
public:
    virtual ~GJStoreItem();

    static GJStoreItem* create(int index, int typeID, int unlockType, int price, ShopType shopType);

    GJStoreItem();
    std::string getCurrencyKey();
    bool init(int index, int typeID, int unlockType, int price, ShopType shopType);

    geode::SeedValueRSV m_index;
    geode::SeedValueRSV m_typeID;
    geode::SeedValueRSV m_unlockType;
    geode::SeedValueRSV m_price;
    ShopType m_shopType;

};
