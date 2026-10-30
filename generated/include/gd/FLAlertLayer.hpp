#pragma once

class ButtonSprite;
class CCTextInputNode;
class FLAlertLayerProtocol;
class ScrollingLayer;
class cocos2d;

class FLAlertLayer : public cocos2d::CCLayerColor, public TextInputDelegate {
public:
    virtual void onEnter();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void rebuildContent();
    virtual void show();
    virtual bool ownsTouchPrio();
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);

    static FLAlertLayer* create(char const* title, const std::string& desc, char const* btn);
    static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, std::string desc, char const* btn1, char const* btn2);
    static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, std::string desc, char const* btn1, char const* btn2, float width);
    static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, std::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale);

    FLAlertLayer();
    ~FLAlertLayer();
    void /* TodoReturn */ addCloseButton();
    void /* TodoReturn */ cancelKeyboardSlide();
    void /* TodoReturn */ containsNode(cocos2d::CCNode*);
    void incrementForcePrio();
    bool init(int opacity);
    bool init(FLAlertLayerProtocol* delegate, char const* title, std::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale);
    void /* TodoReturn */ keyboardRaise(float, float);
    void onButton(cocos2d::CCObject* sender);
    void onButtonTag(int tag);
    void /* TodoReturn */ ownedTouchPrio(int);
    void /* TodoReturn */ rebuildContentDeferred(float);
    void /* TodoReturn */ requestRebuild();

    cocos2d::CCMenu* m_buttonMenu;
    FLAlertLayerProtocol* m_alertProtocol;
    cocos2d::CCNode* m_scene;
    bool m_reverseKeyBack;
    cocos2d::ccColor3B m_color;
    cocos2d::CCLayer* m_mainLayer;
    int m_ZOrder;
    bool m_noElasticity;
    cocos2d::ccColor3B m_color2;
    cocos2d::CCPoint m_unk258;
    ButtonSprite* m_button1;
    ButtonSprite* m_button2;
    ScrollingLayer* m_scrollingLayer;
    int m_controlConnected;
    bool m_containsBorder;
    bool m_noAction;
    int m_joystickConnected;
    bool m_forcePrioRegistered;
    float m_width;
    float m_height;
    bool m_unk290;

};
