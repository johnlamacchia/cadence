#pragma once

class CCIndexPath;
class TableViewCell;
class TableViewCellDelegate;
class TableViewDataSource;
class TableViewDelegate;
class cocos2d;

class TableView : public CCScrollLayerExt, public CCScrollLayerExtDelegate {
public:
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual bool scrollWheel(float y, float x);
    virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt* layer);
    virtual void scrollViewDidEndDecelerating(CCScrollLayerExt* layer);
    virtual void scrollViewTouchMoving(CCScrollLayerExt* layer);
    virtual void scrollViewDidEndMoving(CCScrollLayerExt* layer);

    static TableView* create(TableViewDelegate* tvd, TableViewDataSource* tvds, TableViewCellDelegate* tvcd, cocos2d::CCRect rect);

    TableView(cocos2d::CCRect rect);
    ~TableView();
    void cancelAndStoleTouch(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    TableViewCell* cellForRowAtIndexPath(CCIndexPath& path);
    TableViewCell* cellForTouch(cocos2d::CCTouch* touch);
    int checkBoundaryOfCell(TableViewCell* cell);
    int checkBoundaryOfCell(cocos2d::CCPoint& position, float height);
    void checkBoundaryOfContent(float dt);
    void checkFirstCell(TableViewCell* cell);
    void checkLastCell(TableViewCell* cell);
    void claimTouch(cocos2d::CCTouch* touch);
    void deleteTableViewCell(TableViewCell* cell);
    bool dequeueReusableCellWithIdentifier(char const* identifier);
    void initTableViewCells();
    bool isDuplicateIndexPath(CCIndexPath& path);
    bool isDuplicateInVisibleCellArray(CCIndexPath* path);
    void reloadData();
    bool removeIndexPathFromPathAddedArray(CCIndexPath& path);
    void scrollToIndexPath(CCIndexPath& path);
    void touchFinish(cocos2d::CCTouch* touch);

    bool m_scheduled;
    cocos2d::CCTouch* m_beginTouch;
    cocos2d::CCPoint m_beginLocation;
    cocos2d::CCPoint m_lastCellPos;
    cocos2d::CCPoint m_checkLocation;
    TableViewCell* m_touchedCell;
    bool m_touchMoved;
    bool m_isTouch;
    cocos2d::CCArray* m_cellArray;
    cocos2d::CCArray* m_cellRemovedArray;
    cocos2d::CCArray* m_indexPathArray;
    TableViewDelegate* m_tableDelegate;
    TableViewDataSource* m_dataSource;
    TableViewCellDelegate* m_cellDelegate;
    bool m_unused2;
    void* m_unused3;
    int m_unused4;
    float m_touchLastY;
    bool m_cancellingTouches;
    bool m_idk2;

};
