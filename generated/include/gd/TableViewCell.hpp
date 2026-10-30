#pragma once

class CCIndexPath;
class TableView;
class cocos2d;

class TableViewCell : public cocos2d::CCLayer {
public:
    TableViewCell(char const* identifier, float width, float height);
    ~TableViewCell();
    void updateVisibility();

    cocos2d::CCNode* m_deleteMenu;
    TableView* m_tableView;
    CCIndexPath m_indexPath;
    bool m_editable;
    std::string m_cellIdentifier;
    float m_width;
    float m_height;
    cocos2d::CCLayerColor* m_backgroundLayer;
    cocos2d::CCLayer* m_mainLayer;
    BoomListType m_listType;

};
