#pragma once

class CCMenuItemSpriteExtra;
class CustomSongWidget;
class FLAlertLayer;
class LevelSettingsObject;
class cocos2d;

class SongSelectNode : public cocos2d::CCNode, public FLAlertLayerProtocol, public CustomSongLayerDelegate, public CustomSongDelegate {
public:
    virtual ~SongSelectNode();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void customSongLayerClosed();
    virtual void songIDChanged(int id);
    virtual int getActiveSongID();
    virtual std::string getSongFileName();
    virtual LevelSettingsObject* getLevelSettings();

    static SongSelectNode* create(int id, bool customSong, LevelSettingsObject* settings, SongSelectType selectType, cocos2d::CCPoint position, cocos2d::CCNode* parent, cocos2d::CCMenu* menu, bool noLabel);

    void audioNext(cocos2d::CCObject* sender);
    void audioPrevious(cocos2d::CCObject* sender);
    bool init(int id, bool customSong, LevelSettingsObject* settings, SongSelectType selectType, cocos2d::CCPoint position, cocos2d::CCNode* parent, cocos2d::CCMenu* menu, bool noLabel);
    void onOpenCustomSong(cocos2d::CCObject* sender);
    void onSongMode(cocos2d::CCObject* sender);
    void onSongMode(int songMode);
    void selectSong(int id);
    void showCustomSongSelect();
    void updateAudioLabel();
    void updateWidgetVisibility();

    int m_selectedSongID;
    bool m_isCustomSong;
    bool m_songChanged;
    cocos2d::CCLabelBMFont* m_audioLabel;
    cocos2d::CCArray* m_selectSongObjects;
    cocos2d::CCArray* m_normalSongObjects;
    cocos2d::CCArray* m_changeSongObjects;
    CCMenuItemSpriteExtra* m_normalButton;
    CCMenuItemSpriteExtra* m_customButton;
    CCMenuItemSpriteExtra* m_selectButton;
    CCMenuItemSpriteExtra* m_changeButton;
    CustomSongWidget* m_songWidget;
    LevelSettingsObject* m_settingsObject;

};
