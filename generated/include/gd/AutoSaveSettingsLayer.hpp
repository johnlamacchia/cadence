#pragma once

class cocos2d;

class AutoSaveSettingsLayer : public SetupTriggerPopup {
public:
    virtual ~AutoSaveSettingsLayer();
    virtual bool init();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);
    virtual float getValue(int);

    static AutoSaveSettingsLayer* create();

    void /* TodoReturn */ getBackupCap();
    void /* TodoReturn */ getBackupDirectory();
    void /* TodoReturn */ getCustomBackupDirectory();
    void onAdvanced(cocos2d::CCObject* sender);
    void onOpenBackupDir(cocos2d::CCObject* sender);
    void onSelectBackupDir(cocos2d::CCObject* sender);
    void /* TodoReturn */ openBackupDirectory();
    void setBackupCap(int);
    void setBackupDirectory(std::string);
    void /* TodoReturn */ updateBackupDirLabel();

};
