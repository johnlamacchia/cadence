#pragma once

class FLAlertLayer;
class LevelEditorLayer;
class PauseLayer;
class Slider;
class cocos2d;

class EditorPauseLayer : public CCBlockLayer, public FLAlertLayerProtocol, public VolumeChangeDelegate {
public:
    virtual ~EditorPauseLayer();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void customSetup();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void volumeChanged();

    static EditorPauseLayer* create(LevelEditorLayer* layer);

    EditorPauseLayer();
    void editorSFXSliderChanged(cocos2d::CCObject* sender);
    bool init(LevelEditorLayer* layer);
    void musicSliderChanged(cocos2d::CCObject* sender);
    void onAutoSaveSettings(cocos2d::CCObject* sender);
    void onControls(cocos2d::CCObject* sender);
    void onDeathStats(cocos2d::CCObject* sender);
    void onDownloadAudio(cocos2d::CCObject* sender);
    void onEditorSounds(cocos2d::CCObject* sender);
    void onExitEditor(cocos2d::CCObject* sender);
    void onExitNoSave(cocos2d::CCObject* sender);
    void onExport(cocos2d::CCObject* sender);
    void onHelp(cocos2d::CCObject* sender);
    void onKeybindings(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onResume(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void onSaveAndExit(cocos2d::CCObject* sender);
    void onSaveAndPlay(cocos2d::CCObject* sender);
    void playStep2();
    void playStep3();
    void saveLevel();
    void sfxSliderChanged(cocos2d::CCObject* sender);
    void toggleDebugDraw(cocos2d::CCObject* sender);
    void toggleEditorBackground(cocos2d::CCObject* sender);
    void toggleEditorColorMode(cocos2d::CCObject* sender);
    void toggleEditorGrid(cocos2d::CCObject* sender);
    void toggleEditorGround(cocos2d::CCObject* sender);
    void toggleEffectDuration(cocos2d::CCObject* sender);
    void toggleEffectLines(cocos2d::CCObject* sender);
    void toggleFollowPlayer(cocos2d::CCObject* sender);
    void toggleGridOnTop(cocos2d::CCObject* sender);
    void toggleHideInvisible(cocos2d::CCObject* sender);
    void toggleIgnoreDamage(cocos2d::CCObject* sender);
    void togglePlaytestMusic(cocos2d::CCObject* sender);
    void togglePreviewAnim(cocos2d::CCObject* sender);
    void togglePreviewParticles(cocos2d::CCObject* sender);
    void togglePreviewShaders(cocos2d::CCObject* sender);
    void toggleRecordOrder(cocos2d::CCObject* sender);
    void toggleSelectFilter(cocos2d::CCObject* sender);
    void toggleShowObjectInfo(cocos2d::CCObject* sender);

    bool m_saved;
    LevelEditorLayer* m_editorLayer;
    char _pad_2[32];
    Slider* m_musicSlider;
    Slider* m_sfxSlider;
    Slider* m_editorSFXSlider;

};
