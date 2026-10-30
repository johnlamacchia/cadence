#pragma once

class GJComment;
class GJCommentListLayer;
class LoadingCircle;
class LoadingCircleSprite;
class TableView;
class TableViewCell;
class cocos2d;

class AudioAssetsBrowser : public FLAlertLayer, public TableViewCellDelegate, public MusicDownloadDelegate {
public:
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void musicActionFinished(GJMusicAction action);
    virtual void musicActionFailed(GJMusicAction action);
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual int getSelectedCellIdx();
    virtual int getCellDelegateType();

    static AudioAssetsBrowser* create(std::vector<int>& songIds, std::vector<int>& sfxIds);

    AudioAssetsBrowser();
    ~AudioAssetsBrowser();
    bool init(std::vector<int>& songIds, std::vector<int>& sfxIds);
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onPage(cocos2d::CCObject* sender);
    void setupList();
    void trySetupAudioBrowser();
    void updatePageLabel();

    cocos2d::CCArray* m_songInfoObjects;
    GJCommentListLayer* m_songList;
    cocos2d::CCLabelBMFont* m_pageIndicatorLabel;
    std::vector<int> m_songsIds;
    std::vector<int> m_sfxIds;
    LoadingCircleSprite* m_loadingCircle;

};
