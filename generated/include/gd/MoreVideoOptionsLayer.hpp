#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class OptionsLayer;
class VideoOptionsLayer;
class cocos2d;

class MoreVideoOptionsLayer : public FLAlertLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static MoreVideoOptionsLayer* create();

    MoreVideoOptionsLayer();
    ~MoreVideoOptionsLayer();
    void addToggle(char const* label, char const* key, char const* description);
    int countForPage(int page);
    void goToPage(int page);
    void incrementCountForPage(int page);
    const char* infoKey(int index);
    cocos2d::CCLayer* layerForPage(int page);
    const char* layerKey(int page);
    cocos2d::CCPoint nextPosition(int page);
    const char* objectKey(int page);
    cocos2d::CCArray* objectsForPage(int page);
    void onApplyFPS(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    const char* pageKey(int page);
    void updateFPSButtons();

    int m_page;
    int m_toggleCount;
    int m_pageCount;
    cocos2d::CCDictionary* m_values;
    cocos2d::CCDictionary* m_variables;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCTextInputNode* m_fpsInput;
    cocos2d::CCArray* m_fpsNodes;

};
