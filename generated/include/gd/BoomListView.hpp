#pragma once

class CCIndexPath;
class TableView;
class TableViewCell;
class TableViewCellDelegate;
class cocos2d;

class BoomListView : public cocos2d::CCLayer, public TableViewDelegate, public TableViewDataSource {
public:
    virtual void draw();
    virtual void setupList(float yOffset);
    virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath& indexPath, TableViewCell* cell, TableView* tableView);
    virtual float cellHeightForRowAtIndexPath(CCIndexPath& indexPath, TableView* tableView);
    virtual void didSelectRowAtIndexPath(CCIndexPath& indexPath, TableView* tableView);
    virtual int numberOfRowsInSection(unsigned int section, TableView* tableView);
    virtual unsigned int numberOfSectionsInTableView(TableView* tableView);
    virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath& indexPath, TableView* tableView);
    virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView* tableView, TableViewCellEditingStyle style, CCIndexPath& indexPath);
    virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath& indexPath, TableViewCell* cell, TableView* tableView);
    virtual TableViewCell* getListCell(char const* identifier);
    virtual void loadCell(TableViewCell* cell, int index);

    static BoomListView* create(cocos2d::CCArray* entries, TableViewCellDelegate* delegate, float height, float width, int page, BoomListType type, float y);

    BoomListView();
    ~BoomListView();
    void addObjectToList(cocos2d::CCNode* node);
    bool init(cocos2d::CCArray* entries, BoomListType type, float width, float height);
    bool init(cocos2d::CCArray* entries, TableViewCellDelegate* delegate, float height, float width, int page, BoomListType type, float y);
    void lockList(bool locked);
    void /* TodoReturn */ restoreListOffset(float);

    TableView* m_tableView;
    cocos2d::CCArray* m_entries;
    BoomListType m_type;
    float m_height;
    float m_width;
    float m_itemSeparation;
    int m_currentPage;
    bool m_locked;

};
