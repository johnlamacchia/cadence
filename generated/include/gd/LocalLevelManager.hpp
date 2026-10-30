#pragma once

class DS_Dictionary;
class GJGameLevel;
class cocos2d;

class LocalLevelManager : public GManager, public PlatformFileDelegate {
public:
    virtual ~LocalLevelManager();
    virtual bool init();
    virtual void encodeDataTo(DS_Dictionary* dict);
    virtual void didEncodeForSave();
    virtual void saveFinished(bool);
    virtual void dataLoaded(DS_Dictionary* dict);
    virtual void firstLoad();
    virtual void filesPicked(std::vector<std::string> const&);
    virtual void fileShareFinished(bool);

    static LocalLevelManager* get();
    static LocalLevelManager* sharedState();

    void /* TodoReturn */ exportLevelsToFile(cocos2d::CCArray*);
    cocos2d::CCDictionary* getAllLevelsInDict();
    cocos2d::CCArray* getAllLevelsWithName(std::string name);
    cocos2d::CCArray* getCreatedLevels(int folder);
    cocos2d::CCArray* getCreatedLists(int folder);
    cocos2d::CCDictionary* getLevelsInNameGroups();
    std::string getMainLevelString(int id);
    int importLevelsFromFile(int);
    int importLevelsFromPaths(std::vector<std::string> const&, int);
    void markLevelsAsUnmodified();
    void /* TodoReturn */ moveLevelToBottom(GJGameLevel*);
    void moveLevelToTop(GJGameLevel* level);
    void /* TodoReturn */ postFileResults(float);
    void /* TodoReturn */ queueFileResults();
    void reorderLevels();
    void reorderLists();
    void tryLoadMainLevelString(int id);
    void /* TodoReturn */ tryRecoverAutoSaves();
    int updateLevelOrder();
    void updateLevelRevision();
    int updateListOrder();
    void setLocalLists(cocos2d::CCArray*);
    void setLocalLevels(cocos2d::CCArray*);

    cocos2d::CCArray* m_localLevels;
    cocos2d::CCArray* m_localLists;
    std::unordered_map<int, std::string> m_mainLevels;
    std::vector<std::string> m_importedLevelPaths;
    std::vector<std::pair<std::string, int>> m_platformerJournalKeys;
    std::mutex m_fileMutex;
    std::string m_saveFileName;
    std::string m_savePath;
    std::string m_importPath;
    bool m_unkFileBool1;
    bool m_unkFileBool2;
    int m_unkFileInt1;
    int m_unkFileInt2;
    int m_unkFileInt3;
    int m_unkFileInt4;

};
