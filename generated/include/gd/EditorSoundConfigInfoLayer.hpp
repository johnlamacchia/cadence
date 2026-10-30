#pragma once

class CCTextInputNode;
class EditorSoundBrowserLayer;
class FLAlertLayer;
class GJEditorSoundConfig;
class InfoLayer;
class UploadActionPopup;
class cocos2d;

class EditorSoundConfigInfoLayer : public SetupTriggerPopup, public EditorSoundDownloadDelegate, public EditorSoundDeleteDelegate, public FLAlertLayerProtocol, public AdminActionConfirmDelegate, public LikeItemDelegate, public SoundPackDescDelegate, public UploadActionDelegate, public UploadPopupDelegate, public MusicDownloadDelegate {
public:
    virtual ~EditorSoundConfigInfoLayer();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode*);
    virtual void adminActionConfirmed(int, std::vector<int> const&, std::string const&);
    virtual void downloadEditorSoundFinished(GJEditorSoundConfig*);
    virtual void downloadEditorSoundFailed(int, int);
    virtual void deleteEditorSoundFinished(int);
    virtual void deleteEditorSoundFailed(int, int);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void likedItem(LikeItemType, int, bool);
    virtual void soundPackDescSaved(std::string const&, std::string const&);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void musicActionFinished(GJMusicAction);
    virtual void musicActionFailed(GJMusicAction);

    static EditorSoundConfigInfoLayer* create(GJEditorSoundConfig*, EditorSoundInfoKind, EditorSoundBrowserLayer*);

    void /* TodoReturn */ applyConfig(bool, bool);
    void /* TodoReturn */ buildSummary();
    void /* TodoReturn */ downloadMissingSFX();
    void /* TodoReturn */ editsServerDescription();
    bool init(GJEditorSoundConfig*, EditorSoundInfoKind, EditorSoundBrowserLayer*);
    bool isOwner();
    void /* TodoReturn */ missingSFXIDs();
    void onDeleteServer(cocos2d::CCObject* sender);
    void onDescription(cocos2d::CCObject* sender);
    void onDownloadSFX(cocos2d::CCObject* sender);
    void onDuplicate(cocos2d::CCObject* sender);
    void onFavorite(cocos2d::CCObject* sender);
    void onFeatureAdmin(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onLike(cocos2d::CCObject* sender);
    void onLoad(cocos2d::CCObject* sender);
    void onLocalDelete(cocos2d::CCObject* sender);
    void onMerge(cocos2d::CCObject* sender);
    void onOverwrite(cocos2d::CCObject* sender);
    void onShowDescription(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void onUpload(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshActionButtons();
    void /* TodoReturn */ refreshLikeState();
    void /* TodoReturn */ reloadWindow();
    void /* TodoReturn */ startDownload();
    void /* TodoReturn */ startServerDelete(std::string const&, std::vector<int> const&);
    void /* TodoReturn */ startSFXDownload();

};
