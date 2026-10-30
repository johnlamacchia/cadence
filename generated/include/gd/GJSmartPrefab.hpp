#pragma once

class DS_Dictionary;

class GJSmartPrefab : public cocos2d::CCObject {
public:
    virtual ~GJSmartPrefab();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();

    static GJSmartPrefab* create();
    static GJSmartPrefab* createWithCoder(DS_Dictionary* dict);

    void dataLoaded(DS_Dictionary* dict);
    bool init();

    std::string m_prefabKey;
    std::string m_prefabData;
    int m_prefabID;
    int m_prefabChance;

};
