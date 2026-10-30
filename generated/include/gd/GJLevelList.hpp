#pragma once

class DS_Dictionary;
class GJGameLevel;
class cocos2d;

class GJLevelList : public cocos2d::CCNode {
public:
    virtual ~GJLevelList();
    virtual void encodeWithCoder(DS_Dictionary* dict);
    virtual bool canEncode();
    virtual bool init();

    static GJLevelList* create();
    static GJLevelList* create(cocos2d::CCDictionary* dict);
    static GJLevelList* createWithCoder(DS_Dictionary* dict);
    static std::string frameForListDifficulty(int diff, DifficultyIconType type);

    GJLevelList();
    void addLevelToList(GJGameLevel* level);
    int completedLevels();
    void dataLoaded(DS_Dictionary* dict);
    void duplicateListLevels(GJLevelList* list);
    cocos2d::CCArray* getListLevelsArray(cocos2d::CCArray* levels);
    std::string getUnpackedDescription();
    void handleStatsConflict(GJLevelList* list);
    bool hasMatchingLevels(GJLevelList* list);
    int orderForLevel(int id);
    void parseListLevels(std::string str);
    void removeLevelFromList(int id);
    void reorderLevel(int levelID, int newPosition);
    void reorderLevelStep(int id, bool up);
    void showListInfo();
    int totalLevels();
    void updateLevelsString();

    std::vector<int> m_levels;
    int m_listID;
    int m_listVersion;
    int m_downloads;
    int m_likes;
    int m_difficulty;
    int m_accountID;
    int m_folder;
    int m_listRevision;
    int m_listOrder;
    int m_original;
    int m_diamonds;
    int m_levelsToClaim;
    bool m_isEditable;
    bool m_unlisted;
    bool m_friendsOnly;
    bool m_uploaded;
    bool m_favorite;
    bool m_featured;
    bool m_onlineLevelsLoaded;
    bool m_modified;
    int m_unk;
    std::string m_creatorName;
    std::string m_listName;
    std::string m_unkString;
    std::string m_levelsString;
    std::string m_listDesc;
    int m_uploadDate;
    int m_updateDate;
    cocos2d::CCDictionary* m_levelsDict;
    GJLevelType m_listType;
    int m_M_ID;

};
