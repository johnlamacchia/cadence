#pragma once

class FLAlertLayer;
class GJEditorSoundConfig;
class UploadActionPopup;
class cocos2d;

class GJRateEditorSoundLayer : public SetupTriggerPopup, public UploadPopupDelegate, public UploadActionDelegate, public FLAlertLayerProtocol {
public:
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomButton(cocos2d::CCObject* sender);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static GJRateEditorSoundLayer* create(GJEditorSoundConfig*);

    bool init(GJEditorSoundConfig*);
    void onSubmit(cocos2d::CCObject* sender);

};
