#pragma once

class BoomScrollLayerDelegate;
class DynamicScrollDelegate;
class ExtendedLayer;
class cocos2d;

class BoomScrollLayer : public cocos2d::CCLayer, public cocos2d::CCTouchClipper {
public:
    virtual void visit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual bool touchInsideClip(cocos2d::CCPoint const&);

    static BoomScrollLayer* create(cocos2d::CCArray* pages, int offset, bool looped);
    static BoomScrollLayer* create(cocos2d::CCArray* pages, int offset, bool looped, cocos2d::CCArray* dynamicObjects, DynamicScrollDelegate* delegate);

    BoomScrollLayer();
    ~BoomScrollLayer();
    void addPage(cocos2d::CCLayer* page);
    void addPage(cocos2d::CCLayer* page, int index);
    void cancelAndStoleTouch(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void claimTouch(cocos2d::CCTouch* touch);
    cocos2d::CCLayer* getPage(int page);
    int getRelativePageForNum(int page);
    cocos2d::CCPoint getRelativePosForPage(int page);
    int getTotalPages();
    bool init(cocos2d::CCArray* pages, int offset, bool looped, cocos2d::CCArray* dynamicObjects, DynamicScrollDelegate* delegate);
    void instantMoveToPage(int page);
    void moveToPage(int page);
    void moveToPageEnded();
    int pageNumberForPosition(cocos2d::CCPoint position);
    cocos2d::CCPoint positionForPageWithNumber(int page);
    void quickUpdate();
    void removePage(cocos2d::CCLayer* page);
    void removePageWithNumber(int page);
    void repositionPagesLooped();
    void selectPage(int page);
    void setDotScale(float scale);
    void setPagesIndicatorPosition(cocos2d::CCPoint pos);
    void setupDynamicScrolling(cocos2d::CCArray* dynamicObjects, DynamicScrollDelegate* delegate);
    void togglePageIndicators(bool visible);
    void updateDots(float dt);
    void updatePages();

    cocos2d::CCArray* m_dots;
    int m_slowPage;
    float m_touchX;
    float m_touchQuotient;
    bool m_looped;
    DynamicScrollDelegate* m_dynamicDelegate;
    cocos2d::CCArray* m_dynamicObjects;
    bool m_dynamic;
    int m_touchType;
    bool m_ignoreTouchCancel;
    cocos2d::CCTouch* m_touch;
    cocos2d::CCArray* m_pages;
    double m_touchTime;
    cocos2d::CCPoint m_position;
    ExtendedLayer* m_extendedLayer;
    cocos2d::CCRect m_rect;
    float m_unkFloat1;
    float m_maxSpeed;
    float m_minSpeed;
    float m_unkFloat2;
    BoomScrollLayerDelegate* m_delegate;
    bool m_pageMoving;
    bool m_pagesInvisible;
    float m_minimumTouchLengthToSlide;
    float m_minimumTouchLengthToChangePage;
    float m_width;
    bool m_cancelAndStealTouch;
    bool m_dotsVisible;
    cocos2d::CCPoint m_dotPosition;
    cocos2d::ccColor4B m_selectedPageColor;
    cocos2d::ccColor4B m_normalPageColor;
    int m_page;
    float m_pageOffset;
    void* m_unkPtr;
    bool m_doVisit;
    cocos2d::CCRect m_clippingRect;

};
