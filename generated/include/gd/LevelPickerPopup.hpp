#pragma once

class CCTextInputNode;
class LevelPickerDelegate;
class TableView;
class TableViewCell;
class cocos2d;

class LevelPickerPopup : public PagedListPopup, public LevelManagerDelegate, public TableViewCellDelegate {
public:
    virtual ~LevelPickerPopup();
    virtual int requestPage(int);
    virtual bool cellPerformedAction(TableViewCell*, int, CellAction, cocos2d::CCNode*);
    virtual void loadLevelsFinished(cocos2d::CCArray*, char const*, int);
    virtual void loadLevelsFailed(char const*, int);
    virtual void setupPageInfo(std::string, char const*);
    virtual void textChanged(CCTextInputNode*);

    static LevelPickerPopup* create(LevelPickerDelegate*);

    void /* TodoReturn */ applyKind();
    void /* TodoReturn */ buildSavedPage();
    void /* TodoReturn */ doPageRequest(float);
    bool init(LevelPickerDelegate*);
    void onOnlineMode(cocos2d::CCObject* sender);
    void onRefreshOnline(cocos2d::CCObject* sender);
    void onToggleFavorites(cocos2d::CCObject* sender);
    void onToggleKind(cocos2d::CCObject* sender);
    void onUseID(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshUseID();
    void /* TodoReturn */ savedIndex();
    void /* TodoReturn */ setupList(cocos2d::CCArray*);

};
