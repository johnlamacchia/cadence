#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GJComment;
class GJCommentListLayer;
class LoadingCircle;
class LoadingCircleSprite;
class MusicBrowserDelegate;
class MusicSearchResult;
class SelectSFXSortLayer;
class Slider;
class TableView;
class TableViewCell;
class cocos2d;

class MusicBrowser : public FLAlertLayer, public MusicDownloadDelegate, public TableViewCellDelegate, public FLAlertLayerProtocol, public SliderDelegate, public SelectSFXSortDelegate {
public:
    virtual ~MusicBrowser();
    virtual void update(float dt);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void musicActionFinished(GJMusicAction action);
    virtual void musicActionFailed(GJMusicAction action);
    virtual void sliderEnded(Slider* slider);
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual int getSelectedCellIdx();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void textChanged(CCTextInputNode*);
    virtual void sortSelectClosed(SelectSFXSortLayer*);

    static MusicBrowser* create(int songID, GJSongType songType);

    bool init(int songID, GJSongType songType);
    void onArtistFilters(cocos2d::CCObject* sender);
    void onChangelog(cocos2d::CCObject* sender);
    void onClearSearchText(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onPage(cocos2d::CCObject* sender);
    void onPlaybackControl(cocos2d::CCObject* sender);
    void onSorting(cocos2d::CCObject* sender);
    void onTagFilters(cocos2d::CCObject* sender);
    void onToggleSearch(cocos2d::CCObject* sender);
    void onToggleView(cocos2d::CCObject* sender);
    void onUpdateLibrary(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshLevelIDList();
    void setupList(MusicSearchResult* result);
    void setupMusicBrowser();
    void setupSongControls();
    void sliderChanged(cocos2d::CCObject* sender);
    void trySetupMusicBrowser();
    void updatePageLabel();
    void /* TodoReturn */ updateSongControlLayout();
    void /* TodoReturn */ updateViewItems();

    GJSongType m_songType;
    MusicSearchResult* m_searchResult;
    GJCommentListLayer* m_listLayer;
    cocos2d::CCLabelBMFont* m_infoLabel;
    cocos2d::CCLabelBMFont* m_pageLabel;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_refreshButton;
    CCMenuItemSpriteExtra* m_searchButton;
    CCTextInputNode* m_searchInput;
    cocos2d::extension::CCScale9Sprite* m_searchBackground;
    CCMenuItemSpriteExtra* m_clearSearchButton;
    bool m_searching;
    cocos2d::CCSprite* m_ncsSprite;
    CCMenuItemSpriteExtra* m_tagFilterButton;
    CCMenuItemSpriteExtra* m_artistFilterButton;
    CCMenuItemSpriteExtra* m_favoriteFilterButton;
    CCMenuItemSpriteExtra* m_recentFilterButton;
    CCMenuItemSpriteExtra* m_createdFilterButton;
    LoadingCircleSprite* m_circleSprite;
    int m_songID;
    int m_libraryVersion;
    bool m_gettingURL;
    bool m_selectedCell;
    MusicBrowserDelegate* m_delegate;
    Slider* m_playSlider;
    cocos2d::CCLabelBMFont* m_playLabel;
    CCMenuItemSpriteExtra* m_playButton;
    bool m_autoUpdating;

};
