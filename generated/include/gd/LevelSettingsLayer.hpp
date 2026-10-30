#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class ColorAction;
class ColorChannelSprite;
class LevelEditorLayer;
class LevelSettingsDelegate;
class LevelSettingsObject;
class SelectArtLayer;
class SelectSettingLayer;
class SongSelectNode;
class cocos2d;

class LevelSettingsLayer : public FLAlertLayer, public ColorSelectDelegate, public SelectArtDelegate, public SelectSettingDelegate, public FLAlertLayerProtocol, public CustomSongLayerDelegate {
public:
    virtual ~LevelSettingsLayer();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void colorSelectClosed(cocos2d::CCNode* popup);
    virtual void selectArtClosed(SelectArtLayer* layer);
    virtual void selectSettingClosed(SelectSettingLayer* layer);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static LevelSettingsLayer* create(LevelSettingsObject* object, LevelEditorLayer* layer);

    void createToggleButton(std::string label, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    bool init(LevelSettingsObject* object, LevelEditorLayer* layer);
    void onBGArt(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onCol(cocos2d::CCObject* sender);
    void onDisable(cocos2d::CCObject* sender);
    void onFGArt(cocos2d::CCObject* sender);
    void onGameplayMode(cocos2d::CCObject* sender);
    void onGArt(cocos2d::CCObject* sender);
    void onLiveEdit(cocos2d::CCObject* sender);
    void onMode(cocos2d::CCObject* sender);
    void onOptionToggle(cocos2d::CCObject* sender);
    void onSelectFont(cocos2d::CCObject* sender);
    void onSelectMode(cocos2d::CCObject* sender);
    void onSelectSpeed(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);
    void onShowPicker(cocos2d::CCObject* sender);
    void onSpeed(cocos2d::CCObject* sender);
    void showPicker(ColorAction* action);
    void updateColorSprite(ColorChannelSprite* sprite);
    void updateColorSprites();
    void updateGameplayModeButtons();

    SongSelectNode* m_songSelectNode;
    Speed m_speed;
    ColorChannelSprite* m_bgColorSprite;
    ColorChannelSprite* m_gColorSprite;
    ColorChannelSprite* m_g2ColorSprite;
    ColorChannelSprite* m_lineColorSprite;
    ColorChannelSprite* m_mgColorSprite;
    ColorChannelSprite* m_mg2ColorSprite;
    cocos2d::CCSprite* m_moreColorsSprite;
    cocos2d::CCSprite* m_backgroundSprite;
    cocos2d::CCSprite* m_groundSprite;
    cocos2d::CCSprite* m_middlegroundSprite;
    cocos2d::CCSprite* m_speedSprite;
    cocos2d::CCSprite* m_modeSprite;
    LevelSettingsObject* m_settingsObject;
    void* m_unkPtr;
    cocos2d::CCArray* m_modeToggles;
    cocos2d::CCArray* m_speedButtons;
    LevelSettingsDelegate* m_delegate;
    LevelEditorLayer* m_editorLayer;
    CCTextInputNode* m_orderInput;
    CCTextInputNode* m_channelInput;
    CCMenuItemSpriteExtra* m_classicButton;
    CCMenuItemSpriteExtra* m_platformerButton;
    bool m_disableInput;

};
