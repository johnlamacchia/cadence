#pragma once

class BoomScrollLayer;
class CCMenuItemSpriteExtra;
class GJWorldNode;
class cocos2d;

class WorldSelectLayer : public cocos2d::CCLayer, public BoomScrollLayerDelegate {
public:
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page);
    virtual void scrollLayerScrolledToPage(BoomScrollLayer* layer, int page);
    virtual void scrollLayerMoved(cocos2d::CCPoint position);

    static WorldSelectLayer* create(int page);
    static cocos2d::CCScene* scene(int page);

    void animateInActiveIsland();
    cocos2d::ccColor3B colorForPage(int page);
    cocos2d::ccColor3B getColorValue(int page1, int page2, float progress);
    void goToPage(int page, bool instant);
    bool init(int page);
    void onBack(cocos2d::CCObject* sender);
    void onFreeLevels(cocos2d::CCObject* sender);
    void onGarage(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void onPrev(cocos2d::CCObject* sender);
    void setupWorlds();
    void showCompleteDialog();
    void tryShowAd();
    void unblockButtons();
    void updateArrows();

    cocos2d::CCSprite* m_background;
    BoomScrollLayer* m_scrollLayer;
    CCMenuItemSpriteExtra* m_arrowLeft;
    CCMenuItemSpriteExtra* m_arrowRight;
    bool m_cleanupSprites;
    bool m_enteringLevel;
    GJWorldNode* m_worldNode;
    bool m_buttonsLocked;

};
