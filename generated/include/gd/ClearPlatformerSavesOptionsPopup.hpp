#pragma once

class cocos2d;

class ClearPlatformerSavesOptionsPopup : public SetupTriggerPopup {
public:
    virtual ~ClearPlatformerSavesOptionsPopup();
    virtual bool init();

    static ClearPlatformerSavesOptionsPopup* create();

    void onBackupToggle(cocos2d::CCObject* sender);

};
