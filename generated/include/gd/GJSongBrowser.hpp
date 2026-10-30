#pragma once

class CCMenuItemSpriteExtra;
class CustomListView;
class FLAlertLayer;
class TableView;
class TableViewCell;
class cocos2d;

class GJSongBrowser : public GJDropDownLayer, public FLAlertLayerProtocol, public TableViewCellDelegate {
public:
    virtual ~GJSongBrowser();
    virtual bool init();
    virtual void customSetup();
    virtual void exitLayer(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual int getSelectedCellIdx();

    static GJSongBrowser* create();

    void loadPage(int page);
    void onDeleteAll(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void setupPageInfo(int itemCount, int pageStartIdx, int pageEndIdx);
    void setupSongBrowser(cocos2d::CCArray* songs);

    int m_page;
    int m_songID;
    bool m_selected;
    CCMenuItemSpriteExtra* m_rightArrow;
    CCMenuItemSpriteExtra* m_leftArrow;
    cocos2d::CCLabelBMFont* m_countText;
    cocos2d::CCPoint m_unk298;
    cocos2d::CCArray* m_downloadedSongs;
    CustomListView* m_listView;

};
