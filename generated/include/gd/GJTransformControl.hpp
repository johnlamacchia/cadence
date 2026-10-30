#pragma once

class CCMenuItemSpriteExtra;
class ControlMenuTouch;
class GJTransformControlDelegate;
class GJTransformState;
class GameObject;
class GameObjectEditorState;
class cocos2d;

class GJTransformControl : public cocos2d::CCLayer {
public:
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static GJTransformControl* create();

    GJTransformControl();
    ~GJTransformControl();
    void /* TodoReturn */ applyFreeRotation(float);
    void applyRotation(float rotation);
    void calculateRotationOffset();
    void finishTouch();
    void /* TodoReturn */ getOppositeHandleTag(int);
    void loadFromState(GJTransformState& state);
    void loadValues(GameObject* object, cocos2d::CCArray* objects, std::unordered_map<int, GameObjectEditorState>& states);
    void /* TodoReturn */ moveAnchorTo(cocos2d::CCPoint);
    void onToggleFreeRotation(cocos2d::CCObject* sender);
    void onToggleGridSnap(cocos2d::CCObject* sender);
    void onToggleLockScale(cocos2d::CCObject* sender);
    void onToggleOppositeAnchor(cocos2d::CCObject* sender);
    void refreshControl();
    void saveToState(GJTransformState& state);
    void scaleButtons(float scale);
    void /* TodoReturn */ snapAngleTo45(float);
    void /* TodoReturn */ snapToGrid(cocos2d::CCPoint, bool);
    void /* TodoReturn */ snapToNearestVertex(cocos2d::CCPoint);
    cocos2d::CCSprite* spriteByTag(int tag);
    void updateAnchorSprite(cocos2d::CCPoint position);
    void updateButtons(bool transform, bool skew);
    void updateMinMaxPositions();

    cocos2d::CCNode* m_mainNodeParent;
    cocos2d::CCNode* m_mainNode;
    cocos2d::CCArray* m_objects;
    int m_touchID;
    short m_transformButtonType;
    GJTransformControlDelegate* m_delegate;
    cocos2d::CCPoint m_cursorDifference;
    cocos2d::CCPoint m_topRight;
    cocos2d::CCPoint m_bottomLeft;
    cocos2d::CCPoint m_topRightPosition;
    cocos2d::CCPoint m_bottomLeftPosition;
    cocos2d::CCPoint m_rotationOffset;
    cocos2d::CCPoint m_topRightOrigin;
    cocos2d::CCPoint m_bottomLeftOrigin;
    cocos2d::CCArray* m_warpSprites;
    cocos2d::CCPoint m_rotatePosition;
    CCMenuItemSpriteExtra* m_warpLockButton;
    CCMenuItemSpriteExtra* m_oppositeAnchorButton;
    CCMenuItemSpriteExtra* m_gridSnapButton;
    CCMenuItemSpriteExtra* m_freeRotationButton;
    ControlMenuTouch m_controlMenuTouch;
    float m_scaleX;
    float m_scaleY;
    bool m_warpLocked;
    bool m_oppositeAnchor;
    bool m_gridSnap;
    bool m_freeRotation;
    float m_rotationX;
    float m_rotationY;
    float m_rotation;
    float m_buttonScale;
    cocos2d::CCPoint m_unk26c;
    bool m_anchoring;
    int m_nearestVertex;
    cocos2d::CCPoint m_unk27c;

};
