#pragma once

class BoomScrollLayer;
class CCMenuItemSpriteExtra;
class GJGroundLayer;
class cocos2d;

class LevelSelectLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate, public DynamicScrollDelegate {
public:
    virtual ~LevelSelectLayer();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void updatePageWithObject(cocos2d::CCObject* layer, cocos2d::CCObject* object);
    virtual void scrollLayerMoved(cocos2d::CCPoint position);

    static LevelSelectLayer* create(int page);
    static cocos2d::CCScene* scene(int page);

    LevelSelectLayer();
    cocos2d::ccColor3B colorForPage(int page);
    cocos2d::ccColor3B getColorValue(int page1, int page2, float progress);
    bool init(int page);
    void onBack(cocos2d::CCObject* sender);
    void onDeathTracker(cocos2d::CCObject* sender);
    void onDownload(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void tryShowAd();
    void updateDeathButton(int page);

    float m_width;
    cocos2d::CCSprite* m_backgroundSprite;
    GJGroundLayer* m_groundLayer;
    BoomScrollLayer* m_scrollLayer;
    int m_unknown;
    int m_scrolls;
    bool m_masterDetectiveUnlocked;
    CCMenuItemSpriteExtra* m_deathTrackerButton;

};
