#pragma once

class FLAlertLayer;
class GJGameLevel;
class cocos2d;

class DeathMarkerSettingsPopup : public SetupTriggerPopup, public FLAlertLayerProtocol {
public:
    virtual ~DeathMarkerSettingsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static DeathMarkerSettingsPopup* create(GJGameLevel*);

    bool init(GJGameLevel*);
    void onClearMarkers(cocos2d::CCObject* sender);
    void onFrequencySlider(cocos2d::CCObject* sender);
    void onTimescaleSlider(cocos2d::CCObject* sender);
    void onToggleAnimation(cocos2d::CCObject* sender);
    void onToggleColor(cocos2d::CCObject* sender);
    void onToggleDisabled(cocos2d::CCObject* sender);
    void onToggleGlobal(cocos2d::CCObject* sender);
    void onToggleLabels(cocos2d::CCObject* sender);
    void onToggleLimited(cocos2d::CCObject* sender);
    void onToggleLive(cocos2d::CCObject* sender);
    void onToggleLocal(cocos2d::CCObject* sender);
    void onToggleSound(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateFrequencyLabel(int);
    void /* TodoReturn */ updateTimescaleLabel(int);

};
