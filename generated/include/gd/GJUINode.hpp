#pragma once

class UIButtonConfig;
class cocos2d;

class GJUINode : public cocos2d::CCNode {
public:
    virtual ~GJUINode();
    virtual void draw();

    static GJUINode* create(UIButtonConfig& config);

    int activeRangeTouchTest(cocos2d::CCPoint position);
    int activeTouchTest(cocos2d::CCPoint position);
    float getButtonScale();
    uint8_t getOpacity();
    void highlightButton(int button);
    bool init(UIButtonConfig& config);
    void loadFromConfig(UIButtonConfig& config);
    void resetState();
    void saveToConfig(UIButtonConfig& config);
    void setOpacity(unsigned char opacity);
    void toggleHighlight(int button, bool highlight);
    void toggleModeB(bool modeB);
    void touchEnded();
    int touchTest(cocos2d::CCPoint position);
    void updateButtonFrames();
    void updateButtonPositions();
    void updateButtonScale(float scale);
    void updateDeadzone(int deadzone);
    void updateDragRadius(float radius);
    void updateHeight(float height);
    void updateRangePos(cocos2d::CCPoint position);
    void updateSize(float width, float height);
    void updateWidth(float width);

    cocos2d::CCSprite* m_firstSprite;
    cocos2d::CCSprite* m_secondSprite;
    cocos2d::CCRect m_rect;
    int m_touchID;
    cocos2d::CCPoint m_touchDelta;
    cocos2d::CCPoint m_touchPosition;
    float m_radius;
    float m_deadzone;
    bool m_drawLines;
    bool m_modeB;
    bool m_snap;
    bool m_swiping;
    bool m_moving;
    bool m_player2;
    bool m_oneButton;
    bool m_split;
    PlayerButton m_currentButton;

};
