#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class OptionsLayer;
class cocos2d;

class ParentalOptionsLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static ParentalOptionsLayer* create();

    ParentalOptionsLayer();
    ~ParentalOptionsLayer();
    void /* TodoReturn */ addMasterToggle(char const*, char const*, char const*, char const*);
    void /* TodoReturn */ addToggle(char const*, char const*, int, char const*);
    void /* TodoReturn */ applyPrivacyPushEdge(bool);
    void /* TodoReturn */ applyToggleLock(cocos2d::CCArray*, bool);
    int countForPage(int page);
    void goToPage(int page);
    void incrementCountForPage(int page);
    const char* infoKey(int index);
    cocos2d::CCLayer* layerForPage(int page);
    const char* layerKey(int page);
    cocos2d::CCPoint nextPosition(int page);
    const char* objectKey(int page);
    cocos2d::CCArray* objectsForPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onLockToggle(cocos2d::CCObject* sender);
    void onMasterToggle(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    const char* pageKey(int page);
    void /* TodoReturn */ updateToggleStates();

    int m_page;
    int m_toggleCount;
    int m_maxPage;
    cocos2d::CCDictionary* m_values;
    cocos2d::CCDictionary* m_variables;
    CCMenuItemToggler* m_safeModeToggler;
    CCMenuItemToggler* m_offlineModeToggler;
    CCMenuItemToggler* m_lockToggler;
    cocos2d::CCArray* m_safeModeNodes;
    cocos2d::CCArray* m_toggleNodes;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;

};
