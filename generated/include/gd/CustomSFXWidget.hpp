#pragma once

class CCMenuItemSpriteExtra;
class CustomSFXDelegate;
class FLAlertLayer;
class SFXInfoObject;
class cocos2d;

class CustomSFXWidget : public cocos2d::CCNode, public MusicDownloadDelegate, public FLAlertLayerProtocol {
public:
    virtual ~CustomSFXWidget();
    virtual void downloadSFXFinished(int id);
    virtual void downloadSFXFailed(int id, GJSongError errorType);
    virtual void songStateChanged();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static CustomSFXWidget* create(SFXInfoObject*, CustomSFXDelegate*, bool, bool, bool, bool, bool, bool, bool);

    void deleteSFX();
    void downloadFailed();
    void hideLoadingArt();
    void /* TodoReturn */ infoBtnWidth();
    bool init(SFXInfoObject*, CustomSFXDelegate*, bool, bool, bool, bool, bool, bool, bool);
    void onAddToFolder(cocos2d::CCObject* sender);
    void onCancelDownload(cocos2d::CCObject* sender);
    void onDelete(cocos2d::CCObject* sender);
    void onDownload(cocos2d::CCObject* sender);
    void onEditCustom(cocos2d::CCObject* sender);
    void onPlayback(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);
    void onShowInfo(cocos2d::CCObject* sender);
    void onToggleFavorite(cocos2d::CCObject* sender);
    void onToggleSelect(cocos2d::CCObject* sender);
    void showLoadingArt();
    void startDownload();
    void startMonitorDownload();
    void updateDownloadProgress(float dt);
    void updateError(GJSongError errorType);
    void updateLengthMod(float mod);
    void updatePlaybackBtn();
    void updateProgressBar(int progress);
    void updateSFXInfo();
    void updateSFXObject(SFXInfoObject* object);
    bool verifySFXID(int id);

    SFXInfoObject* m_sfxObject;
    cocos2d::CCMenu* m_buttonMenu;
    cocos2d::CCLabelBMFont* m_titleLabel;
    cocos2d::CCLabelBMFont* m_idLabel;
    cocos2d::CCLabelBMFont* m_errorLabel;
    CCMenuItemSpriteExtra* m_downloadButton;
    CCMenuItemSpriteExtra* m_cancelButton;
    CCMenuItemSpriteExtra* m_selectButton;
    CCMenuItemSpriteExtra* m_playButton;
    CCMenuItemSpriteExtra* m_deleteButton;
    CCMenuItemSpriteExtra* m_toggleFavoriteButton;
    CCMenuItemSpriteExtra* m_addToFolderButton;
    CCMenuItemSpriteExtra* m_editCustomButton;
    CCMenuItemSpriteExtra* m_toggleSelectButton;
    CCMenuItemSpriteExtra* m_infoButton;
    cocos2d::CCSprite* m_progressOutlineSprite;
    cocos2d::CCSprite* m_progressBarSprite;
    cocos2d::CCSprite* m_clockSprite;
    CustomSFXDelegate* m_delegate;
    bool m_showDelete;
    bool m_showPlay;
    bool m_showDownload;
    bool m_showCancel;
    bool m_compactMode;
    bool m_unk1ed;
    bool m_unk1ee;
    int m_sfxID;
    float m_lengthMod;

};
