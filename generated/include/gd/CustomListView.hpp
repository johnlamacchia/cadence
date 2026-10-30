#pragma once

class TableView;
class TableViewCell;
class TableViewCellDelegate;
class cocos2d;

class CustomListView : public BoomListView {
public:
    virtual void setupList(float yOffset);
    virtual TableViewCell* getListCell(char const* identifier);
    virtual void loadCell(TableViewCell* cell, int index);

    static CustomListView* create(cocos2d::CCArray* entries, BoomListType type, float height, float width);
    static CustomListView* create(cocos2d::CCArray* entries, TableViewCellDelegate* delegate, float height, float width, int page, BoomListType type, float y);
    static float getCellHeight(BoomListType type);

    CustomListView();
    void reloadAll();

    int m_cellMode;

};
