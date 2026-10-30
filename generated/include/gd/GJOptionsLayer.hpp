#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class OptionsLayer;
class cocos2d;

class GJOptionsLayer : public SetupTriggerPopup {
public:
    virtual void setupOptions();
    virtual void didToggleGV(std::string variable);
    virtual void didToggle(int toggle);

    static GJOptionsLayer* create(int background);

    GJOptionsLayer();
    ~GJOptionsLayer();
    void addGVToggle(char const* title, char const* variable, char const* description);
    void addToggle(char const* title, int id, bool initial, char const* description);
    void addToggleInternal(char const* title, int id, bool initial, char const* description);
    int countForPage(int page);
    CCMenuItemToggler* getToggleButton(int id);
    void goToPage(int page);
    void incrementCountForPage(int page);
    const char* infoKey(int index);
    bool init(int, float);
    cocos2d::CCLayer* layerForPage(int page);
    const char* layerKey(int page);
    cocos2d::CCPoint nextPosition(int page);
    const char* objectKey(int page);
    cocos2d::CCArray* objectsForPage(int page);
    void offsetToNextPage();
    void onInfo(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    const char* pageKey(int page);

    float m_gap;
    int m_page;
    int m_togglesPerPage;
    int m_toggleCount;
    int m_maxPage;
    float m_maxLabelScale;
    float m_maxLabelWidth;
    cocos2d::CCDictionary* m_values;
    cocos2d::CCDictionary* m_variables;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    float m_offset;
    std::unordered_map<int, CCMenuItemToggler*> m_toggleButtons;

};
