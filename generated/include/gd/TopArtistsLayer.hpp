#pragma once

class CCMenuItemSpriteExtra;
class GJComment;
class GJCommentListLayer;
class LoadingCircle;
class cocos2d;

class TopArtistsLayer : public FLAlertLayer, public OnlineListDelegate {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void loadListFinished(cocos2d::CCArray* objects, char const* key);
    virtual void loadListFailed(char const* key);
    virtual void setupPageInfo(std::string info, char const* key);
    virtual bool ownsTouchPrio();

    static TopArtistsLayer* create();

    TopArtistsLayer();
    ~TopArtistsLayer();
    bool isCorrect(char const* key);
    void loadPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void setupLeaderboard(cocos2d::CCArray* artists);
    void updateLevelsLabel();

    cocos2d::CCArray* m_topArtists;
    GJCommentListLayer* m_listLayer;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    void* m_unkPtr;
    cocos2d::CCLabelBMFont* m_pageInfoLabel;
    int m_itemCount;
    int m_pageStartIdx;
    int m_pageEndIdx;
    int m_unkSize4;
    int m_page;

};
