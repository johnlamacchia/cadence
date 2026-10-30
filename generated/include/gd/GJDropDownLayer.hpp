#pragma once

class GJDropDownLayerDelegate;
class GJListLayer;
class cocos2d;

class GJDropDownLayer : public cocos2d::CCLayerColor {
public:
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

    static GJDropDownLayer* create(const char* title);
    static GJDropDownLayer* create(const char* title, float height, bool noBack);

    GJDropDownLayer();
    ~GJDropDownLayer();
    bool init(char const* title);
    bool init(char const* title, float height, bool noBack);

    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCMenu* m_buttonMenu;
    GJListLayer* m_listLayer;
    cocos2d::CCLayer* m_mainLayer;
    bool m_closeOnHide;
    GJDropDownLayerDelegate* m_delegate;
    bool m_fastMenu;

};
