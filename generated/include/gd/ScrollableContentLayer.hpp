#pragma once

class VirtualRowSource;
class cocos2d;

class ScrollableContentLayer : public cocos2d::CCLayer, public cocos2d::CCTouchClipper {
public:
    virtual ~ScrollableContentLayer();
    virtual void update(float dt);
    virtual void visit();
    virtual bool touchInsideClip(cocos2d::CCPoint const& point);
    virtual bool scrollWheel(float y, float x);

    static ScrollableContentLayer* create(cocos2d::CCSize size);

    void applyContentPosition();
    void clampOffset();
    bool consumePendingCancel();
    void contentWiped();
    cocos2d::CCMenu* createOwnedMenu();
    void ensureOwnedContentNode();
    void ensureScrollThumb();
    void /* TodoReturn */ feedTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ feedTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ feedTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ feedTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    cocos2d::CCNode* getOrCreateSection(int section);
    cocos2d::CCMenu* getOrCreateSectionMenu(int section);
    bool handleTouchBegan(cocos2d::CCTouch* touch);
    void handleTouchCancelled(cocos2d::CCTouch* touch);
    void handleTouchEnded(cocos2d::CCTouch* touch);
    void handleTouchMoved(cocos2d::CCTouch* touch);
    bool init(cocos2d::CCSize);
    void /* TodoReturn */ itemTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ itemTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ itemTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void /* TodoReturn */ itemTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    cocos2d::CCNode* mountedVirtualRow(int row);
    void mountRowSlot(int row);
    void persistScrollOffset(std::string const& key);
    void reloadVirtualRows(float offset);
    void remountVirtualRow(int row);
    bool restoreScrollOffset(std::string const& key);
    void scrollTo(float offset);
    void sectionAddChild(cocos2d::CCNode* child, int zOrder, int tag);
    void sectionClear();
    int sectionIndexForY(float y);
    void setContentHeight(float height);
    void setForwardedTouchesOnly(bool);
    void setHorizontal(bool);
    void setSectionHeight(float);
    void setThumbColor(cocos2d::ccColor3B);
    void setThumbOffset(cocos2d::CCPoint);
    void setThumbOpacity(unsigned char);
    void setViewSize(cocos2d::CCSize size);
    void setVirtualRows(VirtualRowSource* source, float buffer, float topPadding, float bottomPadding);
    void unmountAllRows();
    void unmountRowSlot(int row);
    void updateScrollThumb();
    void updateSectionVisibility();
    void updateVirtualRows();
    void /* TodoReturn */ worldClipRect();

    cocos2d::CCNode* m_content;
    cocos2d::CCSize m_viewSize;
    float m_contentHeight;
    float m_offset;
    int m_touchID;
    cocos2d::CCPoint m_touchStart;
    cocos2d::CCPoint m_touchLast;
    double m_lastMoveTime;
    float m_velocity;
    float m_wheelVelocity;
    bool m_isDragging;
    bool m_pendingCancel;
    cocos2d::CCLayerColor* m_thumb;
    cocos2d::ccColor4B m_thumbColor;
    cocos2d::CCPoint m_thumbOffset;
    bool m_draggingThumb;
    float m_thumbDragStartY;
    float m_thumbDragStartOffset;
    float m_sectionHeight;
    bool m_forwardedTouchesOnly;
    std::map<int, cocos2d::CCNode*> m_sections;
    std::map<int, cocos2d::CCMenu*> m_sectionMenus;
    int m_sectionUnk1;
    int m_sectionUnk2;
    bool m_horizontal;
    VirtualRowSource* m_rowSource;
    float m_rowBuffer;
    float m_rowTopPadding;
    float m_rowBottomPadding;
    std::vector<float> m_rowEdges;
    std::map<int, std::pair<cocos2d::CCNode*, cocos2d::CCMenu*>> m_mountedRows;
    int m_firstMountedRow;
    int m_lastMountedRow;
    bool m_updatingRows;

};
