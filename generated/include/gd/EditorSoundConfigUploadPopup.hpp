#pragma once

class EditorSoundConfigInfoLayer;
class GJEditorSoundConfig;
class InfoLayer;
class UploadPopup;
class cocos2d;

class EditorSoundConfigUploadPopup : public SetupTriggerPopup, public EditorSoundUploadDelegate {
public:
    virtual ~EditorSoundConfigUploadPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void uploadEditorSoundFinished(int);
    virtual void uploadEditorSoundFailed(int);

    static EditorSoundConfigUploadPopup* create(GJEditorSoundConfig*, EditorSoundConfigInfoLayer*);

    bool init(GJEditorSoundConfig*, EditorSoundConfigInfoLayer*);
    void onCancel(cocos2d::CCObject* sender);
    void onToggleFriendsOnly(cocos2d::CCObject* sender);
    void onToggleUnlisted(cocos2d::CCObject* sender);
    void onUpload(cocos2d::CCObject* sender);
    void onUploadAsUpdate(cocos2d::CCObject* sender);
    void /* TodoReturn */ performUpload(int);
    void /* TodoReturn */ updateVisibilityState();

};
