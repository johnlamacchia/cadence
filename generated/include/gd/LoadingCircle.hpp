#pragma once

class cocos2d;

class LoadingCircle : public cocos2d::CCLayerColor {
public:
    virtual ~LoadingCircle();
    virtual bool init();
    virtual void draw();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();

    static LoadingCircle* create();

    LoadingCircle();
    void fadeAndRemove();
    void setFade(bool fade);
    void setParentLayer(cocos2d::CCLayer* layer);
    void show();

    cocos2d::CCSprite* m_sprite;
    cocos2d::CCLayer* m_parentLayer;
    bool m_fade;

};
