#pragma once

class FLAlertLayer;
class GJGameLevel;
class UploadActionPopup;
class cocos2d;

class PlatformerControlsPopup : public SetupTriggerPopup, public UploadActionDelegate, public UploadPopupDelegate, public LevelControlsDelegate, public FLAlertLayerProtocol {
public:
    virtual ~PlatformerControlsPopup();
    virtual void update(float dt);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void onLevelControlsRefreshed(int, int);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static PlatformerControlsPopup* create(GJGameLevel*, bool);

    void /* TodoReturn */ commitCustomDraft();
    void ensureCustomDraft();
    bool init(GJGameLevel*, bool);
    void onApplyCreator(cocos2d::CCObject* sender);
    void onController(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onKeyboard(cocos2d::CCObject* sender);
    void onToggleSource(cocos2d::CCObject* sender);
    void onToggleUpload(cocos2d::CCObject* sender);
    void onTouch(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void onUpload(cocos2d::CCObject* sender);
    void /* TodoReturn */ presentClasses();
    void /* TodoReturn */ reapplyRuntime();
    void /* TodoReturn */ refreshApplyVisible();
    void /* TodoReturn */ refreshClassUI();
    void /* TodoReturn */ refreshModeUI();
    void /* TodoReturn */ refreshUploadColumn(int);
    void /* TodoReturn */ sourceIsCustom();
    void /* TodoReturn */ syncSourceUI();
    void /* TodoReturn */ updateEditLabel();

};
