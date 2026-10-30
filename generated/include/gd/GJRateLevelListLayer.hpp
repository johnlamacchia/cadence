#pragma once

class FLAlertLayer;
class GJGameLevel;
class GJLevelList;
class LevelListLayer;
class UploadActionPopup;
class cocos2d;

class GJRateLevelListLayer : public SetupTriggerPopup, public UploadPopupDelegate, public UploadActionDelegate, public FLAlertLayerProtocol {
public:
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomButton(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static GJRateLevelListLayer* create(GJLevelList*, cocos2d::CCArray*);

    void /* TodoReturn */ createStatPillar(int, int, int, cocos2d::ccColor3B, cocos2d::ccColor3B, cocos2d::CCPoint, std::string);
    void /* TodoReturn */ impliedStarsForLevel(GJGameLevel*);
    bool init(GJLevelList*, cocos2d::CCArray*);
    void onSubmit(cocos2d::CCObject* sender);
    void /* TodoReturn */ starSumForBottom(int);
    void /* TodoReturn */ updateStarSumLabel(int);

};
