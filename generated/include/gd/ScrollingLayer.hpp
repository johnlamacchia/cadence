#pragma once

class cocos2d;

class ScrollingLayer : public cocos2d::CCLayerColor {
public:
    virtual ~ScrollingLayer();
    virtual void draw();
    virtual void visit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static ScrollingLayer* create(cocos2d::CCSize size, cocos2d::CCPoint position, float factor);

    ScrollingLayer();
    cocos2d::CCRect getViewRect();
    bool init(cocos2d::CCSize size, cocos2d::CCPoint position, float factor);
    void setStartOffset(cocos2d::CCPoint offset);

    cocos2d::CCPoint m_position;
    cocos2d::CCSize m_size;
    cocos2d::CCPoint m_touchStartPosition;
    cocos2d::CCPoint m_touchPosition;
    cocos2d::CCPoint m_startOffset;
    float m_scrollFactor;
    int m_touchID;
    cocos2d::CCLayer* m_contentLayer;
    cocos2d::CCLayer* m_parentLayer;

};
