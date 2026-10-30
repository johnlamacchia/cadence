#pragma once

class cocos2d;

class GJMapPack : public cocos2d::CCNode {
public:
    virtual bool init();

    static GJMapPack* create();
    static GJMapPack* create(cocos2d::CCDictionary* dict);

    GJMapPack();
    ~GJMapPack();
    int completedMaps();
    bool hasCompletedMapPack();
    void parsePackColors(std::string textColorStr, std::string barColorStr);
    void parsePackLevels(std::string levelsStr);
    int totalMaps();

    cocos2d::CCArray* m_levels;
    int m_packID;
    GJDifficulty m_difficulty;
    int m_stars;
    int m_coins;
    std::string m_packName;
    std::string m_levelStrings;
    cocos2d::ccColor3B m_textColour;
    cocos2d::ccColor3B m_barColour;
    int m_MId;
    bool m_isGauntlet;

};
