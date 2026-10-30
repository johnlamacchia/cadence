#pragma once

class GJEffectManager;
class GJGameLevel;
class cocos2d;

class LevelSettingsObject : public cocos2d::CCNode {
public:
    virtual ~LevelSettingsObject();
    virtual bool init();

    static LevelSettingsObject* create();
    static LevelSettingsObject* objectFromDict(cocos2d::CCDictionary* dict);
    static LevelSettingsObject* objectFromString(std::string const& str);
    static std::string sanitizeIDName(std::string const& name);

    int findIDByName(std::string const& name, int type);
    void /* TodoReturn */ getCustomSFXPathRaw(int, int);
    std::string getIDName(int id, int type);
    std::unordered_map<int, std::string> const& getIDNames(int type);
    std::vector<int> const& getIDUseOrder(int type);
    std::string getSaveString();
    void removeFromIDUseOrder(int id, int type);
    void setIDName(int type, std::string const& name, int id);
    void setupColorsFromLegacyMode(cocos2d::CCDictionary* dict);
    bool shouldUseYSection();
    void touchIDUsage(int id, int type);

    GJEffectManager* m_effectManager;
    int m_startMode;
    Speed m_startSpeed;
    bool m_startMini;
    bool m_startDual;
    bool m_mirrorMode;
    bool m_rotateGameplay;
    bool m_twoPlayerMode;
    bool m_platformerMode;
    float m_songOffset;
    bool m_fadeIn;
    bool m_fadeOut;
    bool m_dontReset;
    int m_backgroundIndex;
    int m_groundIndex;
    int m_fontIndex;
    int m_middleGroundIndex;
    bool m_startsWithStartPos;
    bool m_isFlipped;
    bool m_reverseGameplay;
    bool m_disableStartPos;
    int m_targetOrder;
    int m_targetChannel;
    GJGameLevel* m_level;
    std::string m_guidelineString;
    bool m_guidelinesUpdated;
    int m_colorPage;
    int m_groundLineIndex;
    bool m_propertykA23;
    bool m_propertykA24;
    bool m_noTimePenalty;
    int m_propertykA44;
    int m_nextFreeID;
    bool m_resetCamera;
    int m_spawnGroup;
    bool m_allowMultiRotation;
    bool m_enablePlayerSqueeze;
    bool m_fixGravityBug;
    bool m_fixNegativeScale;
    bool m_fixRobotJump;
    bool m_dynamicLevelHeight;
    bool m_sortGroups;
    bool m_fixRadiusCollision;
    bool m_enable22Changes;
    bool m_allowStaticRotate;
    bool m_reverseSync;
    bool m_decreaseBoostSlide;
    bool m_enableImpulseFix;

};
