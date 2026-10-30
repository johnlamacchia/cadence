#pragma once

class DialogDelegate;
class DialogObject;
class TextArea;
class cocos2d;

class DialogLayer : public cocos2d::CCLayerColor, public TextAreaDelegate {
public:
    virtual void onEnter();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void fadeInTextFinished(TextArea* textArea);

    static DialogLayer* create(DialogObject* object, int background);
    static DialogLayer* createDialogLayer(DialogObject* object, cocos2d::CCArray* objects, int background);
    static DialogLayer* createWithObjects(cocos2d::CCArray* objects, int background);

    DialogLayer();
    ~DialogLayer();
    void addToMainScene();
    void animateIn(DialogAnimationType type);
    void animateInDialog();
    void animateInRandomSide();
    void displayDialogObject(DialogObject* object);
    void displayNextObject();
    void finishCurrentAnimation();
    void handleDialogTap();
    bool init(DialogObject* object, cocos2d::CCArray* objects, int background);
    void onClose();
    void updateChatPlacement(DialogChatPlacement placement);
    void updateNavButtonFrame();

    double m_animateTime;
    cocos2d::CCLayer* m_mainLayer;
    cocos2d::CCLabelBMFont* m_characterLabel;
    TextArea* m_textArea;
    cocos2d::CCSprite* m_characterSprite;
    cocos2d::CCArray* m_dialogObjects;
    int m_touchID;
    cocos2d::CCSprite* m_navButtonSprite;
    bool m_animating;
    bool m_skippable;
    DialogDelegate* m_delegate;
    bool m_handleTap;
    DialogAnimationType m_animationType;
    bool m_noRemove;

};
