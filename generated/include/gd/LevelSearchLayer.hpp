#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class FLAlertLayer;
class GJSearchObject;
class cocos2d;

class LevelSearchLayer : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol, public DemonFilterDelegate {
public:
    virtual void keyDown(cocos2d::enumKeyCodes, double);
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void demonFilterSelectClosed(int filter);

    static LevelSearchLayer* create(int type);
    static cocos2d::CCScene* scene(int type);

    LevelSearchLayer();
    ~LevelSearchLayer();
    bool checkDiff(int diff);
    bool checkTime(int time);
    void clearFilters();
    void confirmClearFilters(cocos2d::CCObject* sender);
    char const* getDiffKey(int diff);
    std::string getLevelLenKey();
    std::string getSearchDiffKey();
    GJSearchObject* getSearchObject(SearchType type, std::string query);
    char const* getTimeKey(int time);
    bool init(int type);
    void onBack(cocos2d::CCObject* sender);
    void onClearFreeSearch(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onFollowed(cocos2d::CCObject* sender);
    void onFriends(cocos2d::CCObject* sender);
    void onHyped(cocos2d::CCObject* sender);
    void onLatestStars(cocos2d::CCObject* sender);
    void onMagic(cocos2d::CCObject* sender);
    void onMoreOptions(cocos2d::CCObject* sender);
    void onMostDownloaded(cocos2d::CCObject* sender);
    void onMostLikes(cocos2d::CCObject* sender);
    void onMostRecent(cocos2d::CCObject* sender);
    void onSearch(cocos2d::CCObject* sender);
    void onSearchMode(cocos2d::CCObject* sender);
    void onSearchUser(cocos2d::CCObject* sender);
    void onSpecialDemon(cocos2d::CCObject* sender);
    void onStarAward(cocos2d::CCObject* sender);
    void onTrending(cocos2d::CCObject* sender);
    void toggleDifficulty(cocos2d::CCObject* sender);
    void toggleDifficultyNum(int diff, bool enabled);
    void toggleNoStar(cocos2d::CCObject* sender);
    void toggleStar(cocos2d::CCObject* sender);
    void toggleTime(cocos2d::CCObject* sender);
    void toggleTimeNum(int time, bool enabled);
    void updateSearchLabel(char const* label);

    int m_type;
    CCTextInputNode* m_searchInput;
    cocos2d::CCSprite* m_starsSprite;
    cocos2d::CCSprite* m_noStarSprite;
    cocos2d::CCArray* m_difficultySprites;
    cocos2d::CCArray* m_lengthSprites;
    cocos2d::CCDictionary* m_difficultyDict;
    cocos2d::CCSprite* m_lastDifficultySprite;
    CCMenuItemSpriteExtra* m_demonTypeButton;

};
