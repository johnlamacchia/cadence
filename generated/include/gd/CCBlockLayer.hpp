#pragma once

class cocos2d;

class CCBlockLayer : public cocos2d::CCLayerColor {
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
    virtual void exitLayer();
    virtual void showLayer(bool instant);
    virtual void hideLayer(bool instant);
    virtual void layerVisible();
    virtual void layerHidden();
    virtual void enterAnimFinished();
    virtual void disableUI();
    virtual void enableUI();

    static CCBlockLayer* create();

    CCBlockLayer();
    ~CCBlockLayer();
    void decrementForcePrio();
    void incrementForcePrio();

    bool m_closeOnHide;
    bool m_forcePrioRegistered;

};
