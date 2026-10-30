#pragma once

class CCMenuItemSpriteExtra;
class CreateStartPosPopup;
class FLAlertLayer;
class Slider;
class StartPosBrowser;
class StartPosObject;
class TimeSpeedControl;
class cocos2d;

class PauseLayer : public CCBlockLayer, public FLAlertLayerProtocol, public StartPosBrowserDelegate, public CreateStartPosPopupDelegate, public VolumeChangeDelegate {
public:
    virtual ~PauseLayer();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void customSetup();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);
    virtual void volumeChanged();
    virtual bool handleAction(int action, bool down, double timestamp);
    virtual void startPosSelected(StartPosBrowser*, int, StartPosObject*);
    virtual void createStartPosPopupClosed(CreateStartPosPopup*);

    static PauseLayer* create(bool unfocused);

    PauseLayer();
    void checkQuestsStatus();
    void createToggleButton(std::string label, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    StartPosObject* getQueuedStartPos();
    void goEdit();
    bool init(bool unfocused);
    void musicSliderChanged(cocos2d::CCObject* sender);
    void onChangeStartpos(cocos2d::CCObject* sender);
    void onCreateStartpos(cocos2d::CCObject* sender);
    void onEdit(cocos2d::CCObject* sender);
    void onHelp(cocos2d::CCObject* sender);
    void onNormalMode(cocos2d::CCObject* sender);
    void onPracticeMode(cocos2d::CCObject* sender);
    void onQuit(cocos2d::CCObject* sender);
    void onRecordReplays(cocos2d::CCObject* sender);
    void onReplay(cocos2d::CCObject* sender);
    void onRestart(cocos2d::CCObject* sender);
    void onRestartFull(cocos2d::CCObject* sender);
    void onResume(cocos2d::CCObject* sender);
    void onShowStartposBrowser(cocos2d::CCObject* sender);
    void onSideButtonPressed(cocos2d::CCObject* sender);
    void onTime(cocos2d::CCObject* sender);
    void onToggleUI(cocos2d::CCObject* sender);
    void onTryEdit(cocos2d::CCObject* sender);
    void setupLegacyProgressBars();
    void setupPlatformerLabels(bool);
    void setupProgressBars();
    void setupSessionProgressBar();
    void sfxSliderChanged(cocos2d::CCObject* sender);
    void tryQuit(cocos2d::CCObject* sender);
    void tryShowBanner(float dt);

    bool m_unfocused;
    bool m_tryingQuit;
    bool m_uiHidden;
    cocos2d::CCLabelBMFont* m_startposLabel;
    int m_startPosIndex;
    cocos2d::CCArray* m_startPositions;
    bool m_isEditorLevel;
    TimeSpeedControl* m_timeSpeedControls;
    cocos2d::CCLayer* m_layer;
    Slider* m_musicSlider;
    Slider* m_sfxSlider;
    cocos2d::CCMenu* m_showUIMenu;
    CCMenuItemSpriteExtra* m_platformSettingsButton;

};
