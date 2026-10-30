#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class SlideInLayer : public cocos2d::CCLayerColor {
public:
    virtual bool init();
    virtual void draw();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void customSetup();
    virtual void enterLayer();
    virtual void exitLayer(cocos2d::CCObject* sender);
    virtual void showLayer(bool instant);
    virtual void hideLayer(bool instant);
    virtual void layerVisible();
    virtual void layerHidden();
    virtual void enterAnimFinished();
    virtual void disableUI();
    virtual void enableUI();

    static SlideInLayer* create();

    cocos2d::CCLayer* m_mainLayer;
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCSprite* m_exitSprite;
    CCMenuItemSpriteExtra* m_exitButton;
    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    void* m_delegate;
    bool m_closeOnHide;

};
