#pragma once

class CCContentLayer;
class CCScrollLayerExtDelegate;
class cocos2d;

class CCScrollLayerExt : public cocos2d::CCLayer, public cocos2d::CCTouchClipper {
public:
    virtual void visit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void preVisitWithClippingRect(cocos2d::CCRect rect);
    virtual void postVisit();
    virtual bool touchInsideClip(cocos2d::CCPoint const&);

    CCScrollLayerExt(cocos2d::CCRect rect);
    ~CCScrollLayerExt();
    void constraintContent();
    void doConstraintContent(bool instant);
    float getMaxY();
    float getMinY();
    void moveToTop();
    void moveToTopWithOffset(float offset);
    cocos2d::CCRect scissorRect();
    void scrollingEnd();
    void scrollLayer(float offset);
    void setContentLayerSize(cocos2d::CCSize size);
    void setContentOffset(cocos2d::CCPoint offset, bool animate);
    void updateIndicators(float dt);
    void updateScrollDecay(float dt);

    cocos2d::CCTouch* m_touch;
    cocos2d::CCPoint m_touchPosition;
    cocos2d::CCPoint m_touchStartPosition;
    cocos2d::cc_timeval m_timeValue;
    bool m_touchDown;
    bool m_notAtEndOfScroll;
    cocos2d::CCLayerColor* m_verticalScrollbar;
    cocos2d::CCLayerColor* m_horizontalScrollbar;
    CCScrollLayerExtDelegate* m_delegate;
    CCContentLayer* m_contentLayer;
    bool m_cutContent;
    bool m_vScrollbarVisible;
    bool m_hScrollbarVisible;
    bool m_disableHorizontal;
    bool m_disableVertical;
    bool m_disableMovement;
    float m_scrollLimitTop;
    float m_scrollLimitBottom;
    float m_peekLimitTop;
    float m_peekLimitBottom;
    float m_scrollDecay;
    bool m_scrolling;

};
