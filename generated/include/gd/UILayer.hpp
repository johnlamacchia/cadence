#pragma once

class CCMenuItemSpriteExtra;
class GJBaseGameLayer;
class GJUINode;
class LevelControlConfig;
class cocos2d;

class UILayer : public cocos2d::CCLayerColor, public cocos2d::CCKeyboardDelegate {
public:
    virtual void draw();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);
    virtual bool handleAction(int action, bool down, double timestamp);

    static UILayer* create(GJBaseGameLayer* layer);
    static UILayer* get();

    UILayer();
    ~UILayer();
    void announceLevelControls();
    void applyLevelBindings();
    void disableMenu();
    void doPause();
    void editorPlaytest(bool visible);
    void enableEditorMode();
    void enableMenu();
    void handleKeypress(cocos2d::enumKeyCodes key, bool down, double timestamp);
    bool init(GJBaseGameLayer* layer);
    bool isJumpButtonPressed();
    bool isJumpButtonPressed(bool player1);
    void onCheck(cocos2d::CCObject* sender);
    void onDeleteCheck(cocos2d::CCObject* sender);
    void onPause(cocos2d::CCObject* sender);
    bool processUINodesTouch(GJUITouchEvent event, cocos2d::CCTouch* touch);
    bool processUINodeTouch(GJUITouchEvent event, int touchID, cocos2d::CCPoint position, GJUINode* node, double timestamp);
    void reapplyLevelControls();
    void refreshDpad();
    void resetAllButtons();
    void resetUINodeState();
    void resolveLevelControls();
    void showTextAlert(std::string text, bool isOn, bool useRawText, float delay);
    void toggleCheckpointsMenu(bool visible);
    void toggleMenuVisibility(bool visible);
    void togglePlatformerMode(bool platformer);
    void updateCheckState();
    void updateDualMode(bool dual);
    void updateUINodeVisibility(bool visible);

    void* m_pUnknown1;
    cocos2d::CCMenu* m_checkpointMenu;
    CCMenuItemSpriteExtra* m_pauseBtn;
    bool m_bUnknown2;
    bool m_bUnknown3;
    bool m_p1Jumping;
    bool m_p2Jumping;
    bool m_checkpointBtnDown;
    int m_p1TouchId;
    int m_p2TouchId;
    float m_clkTimer;
    bool m_inPlatformer;
    GJBaseGameLayer* m_gameLayer;
    bool m_initialized;
    cocos2d::CCArray* m_uiNodes;
    bool m_dualMode;
    bool m_dpadType;
    bool m_editorMode;
    bool m_editorPlaytest;
    cocos2d::CCArray* m_controllerButtons;
    LevelControlConfig* m_levelControls;
    bool m_didAnnounceLevelControls;

};
