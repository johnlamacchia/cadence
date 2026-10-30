#pragma once

class cocos2d;

class FileSaveManager : public GManager {
public:
    virtual bool init();
    virtual void firstLoad();

    static FileSaveManager* sharedState();

    cocos2d::CCDictionary* getStoreData();
    void loadDataFromFile(char const* filename);

    cocos2d::CCDictionary* m_storeData;

};
