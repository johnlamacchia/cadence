#pragma once

class AutoBackupOptionsDelegate;
class FLAlertLayer;
class cocos2d;

class AutoBackupOptionsPopup : public SetupTriggerPopup, public FLAlertLayerProtocol {
public:
    virtual ~AutoBackupOptionsPopup();
    virtual void valueDidChange(int, float);
    virtual float getValue(int);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static AutoBackupOptionsPopup* create(AutoBackupOptionsDelegate*);

    bool init(AutoBackupOptionsDelegate*);
    void onDeleteAll(cocos2d::CCObject* sender);
    void onPrune(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshStats();

};
