#pragma once

class CreatorSettingsDelegate;
class GJGameLevel;
class SetTextPopup;
class UploadActionPopup;
class cocos2d;

class CreatorSettingsPopup : public SetupTriggerPopup, public UploadActionDelegate, public UploadPopupDelegate, public SetTextPopupDelegate {
public:
    virtual ~CreatorSettingsPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void setTextPopupClosed(SetTextPopup*, std::string);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);

    static CreatorSettingsPopup* create(GJGameLevel*, CreatorSettingsDelegate*);

    bool init(GJGameLevel*, CreatorSettingsDelegate*);
    void onControls(cocos2d::CCObject* sender);
    void onDescription(cocos2d::CCObject* sender);

};
