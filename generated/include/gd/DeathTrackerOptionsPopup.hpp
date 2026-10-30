#pragma once

class DeathTrackerPopup;
class FLAlertLayer;
class GJGameLevel;
class cocos2d;

class DeathTrackerOptionsPopup : public SetupTriggerPopup, public FLAlertLayerProtocol {
public:
    virtual ~DeathTrackerOptionsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static DeathTrackerOptionsPopup* create(GJGameLevel*, DeathTrackerPopup*);

    bool init(GJGameLevel*, DeathTrackerPopup*);
    void onClearData(cocos2d::CCObject* sender);
    void onToggleAutoScroll(cocos2d::CCObject* sender);

};
