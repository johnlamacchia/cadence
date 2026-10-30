#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class Slider;
class cocos2d;

class OptionsLayer : public GJDropDownLayer, public FLAlertLayerProtocol, public VolumeChangeDelegate {
public:
    virtual ~OptionsLayer();
    virtual void customSetup();
    virtual void layerHidden();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void volumeChanged();

    static OptionsLayer* create();

    OptionsLayer();
    CCMenuItemToggler* createToggleButton(std::string text, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    void exitLayer();
    void musicSliderChanged(cocos2d::CCObject* sender);
    void onAccount(cocos2d::CCObject* sender);
    void onHelp(cocos2d::CCObject* sender);
    void onMenuMusic(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onParental(cocos2d::CCObject* sender);
    void onProgressBar(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void onRecordReplays(cocos2d::CCObject* sender);
    void onSecretVault(cocos2d::CCObject* sender);
    void onSoundtracks(cocos2d::CCObject* sender);
    void onSupport(cocos2d::CCObject* sender);
    void onVideo(cocos2d::CCObject* sender);
    void sfxSliderChanged(cocos2d::CCObject* sender);
    void tryEnableRecord();

    cocos2d::CCMenu* m_optionsMenu;
    void* m_unknown;
    int m_layerChoice;
    bool m_recordReplays;
    Slider* m_musicSlider;
    Slider* m_sfxSlider;
    int m_lastVaultDialog;

};
