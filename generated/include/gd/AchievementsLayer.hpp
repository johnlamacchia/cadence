#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class AchievementsLayer : public GJDropDownLayer {
public:
    virtual ~AchievementsLayer();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void customSetup();

    static AchievementsLayer* create();

    AchievementsLayer();
    void loadPage(int page);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void setupLevelBrowser(cocos2d::CCArray* arr);
    void setupPageInfo(int itemCount, int pageStartIdx, int pageEndIdx);

    int m_currentPage;
    CCMenuItemSpriteExtra* m_nextPageButton;
    CCMenuItemSpriteExtra* m_prevPageButton;
    cocos2d::CCLabelBMFont* m_pageLabel;
    cocos2d::CCPoint m_unkPoint;

};
