#pragma once

class CCTextInputNode;
class EditorUI;
class ScrollableContentLayer;
class cocos2d;

class EditorWindowPopup : public SetupTriggerPopup {
public:
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void show();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textInputClosed(CCTextInputNode*);
    virtual bool handleAction(int action, bool down, double timestamp);
    virtual void onBuildWindowContent();
    virtual void onWindowResized();
    virtual void onFieldEnter();

    EditorWindowPopup();
    bool initWindow(EditorUI* editorUI, float width, float height, int windowID, int unused);
    bool beginTouch(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    bool isInCloseButton(cocos2d::CCPoint point);
    void /* TodoReturn */ activeField();
    void /* TodoReturn */ applyWindowSize();
    void /* TodoReturn */ blurField();
    void /* TodoReturn */ clampWindowCenter(cocos2d::CCPoint);
    void /* TodoReturn */ clampWindowOnScreen(cocos2d::CCPoint);
    void /* TodoReturn */ contentCenter();
    void /* TodoReturn */ dismissDropdowns();
    void /* TodoReturn */ fieldHoldsKeys();
    void /* TodoReturn */ getResizeCorner(cocos2d::CCPoint);
    bool isInDragHandle(cocos2d::CCPoint);
    void /* TodoReturn */ layoutWindow();
    void /* TodoReturn */ moveWindowTo(cocos2d::CCPoint);
    void setLockAspectRatio(bool);
    void setWindowScaleBounds(float, float);
    void setWindowSizeBounds(float, float, float, float);
    void /* TodoReturn */ syncWindowCenter();
    void /* TodoReturn */ touchIsOnField(cocos2d::CCPoint);
    void /* TodoReturn */ touchRect();

    EditorUI* m_editorUI;
    cocos2d::extension::CCScale9Sprite* m_background;
    cocos2d::CCSprite* m_closeSprite;
    ScrollableContentLayer* m_scrollContentLayer;
    cocos2d::CCMenu* m_scrollContentMenu;
    float m_windowWidth;
    float m_windowHeight;
    float m_windowScale;
    cocos2d::CCPoint m_windowPosition;
    int m_touchID;
    bool m_draggingWindow;
    bool m_allowDraggingAnywhere;
    bool m_startCentered;
    cocos2d::CCPoint m_dragOffset;
    bool m_contentTouch;
    bool m_menuTouch;
    bool m_closeButtonTouch;
    bool m_closeButtonRelease;
    int m_resizeCorner;
    cocos2d::CCPoint m_resizeAnchor;
    cocos2d::CCPoint m_resizeOffset;
    int m_resizeMode;
    bool m_lockAspectRatio;
    float m_aspectRatio;
    float m_minWidth;
    float m_minHeight;
    float m_maxWidth;
    float m_maxHeight;
    float m_minScale;
    float m_maxScale;
    int m_windowID;
    int m_version;
    bool m_isEmotePicker;

};
