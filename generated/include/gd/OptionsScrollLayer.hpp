#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GJComment;
class GJCommentListLayer;
class MusicSearchResult;
class TableView;
class TableViewCell;
class cocos2d;

class OptionsScrollLayer : public FLAlertLayer, public TableViewCellDelegate {
public:
    virtual ~OptionsScrollLayer();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual void textChanged(CCTextInputNode*);

    static OptionsScrollLayer* create(cocos2d::CCArray*, bool, std::string, MusicSearchResult*);

    void /* TodoReturn */ addTagMatchToggle();
    cocos2d::CCArray* getRelevantObjects(cocos2d::CCArray* objects);
    bool init(cocos2d::CCArray*, bool, std::string, MusicSearchResult*);
    void onClearAll(cocos2d::CCObject* sender);
    void onClearSearchText(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onMatchToggle(cocos2d::CCObject* sender);
    void setupList(cocos2d::CCArray* objects);

    cocos2d::CCArray* m_optionObjects;
    GJCommentListLayer* m_listLayer;
    bool m_recreateList;
    CCTextInputNode* m_searchInput;
    cocos2d::extension::CCScale9Sprite* m_searchBackground;
    CCMenuItemSpriteExtra* m_clearSearchButton;
    std::string m_searchQuery;
    CCMenuItemSpriteExtra* m_matchToggleButton;
    MusicSearchResult* m_searchResult;

};
