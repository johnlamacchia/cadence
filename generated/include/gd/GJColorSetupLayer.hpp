#pragma once

class CCMenuItemSpriteExtra;
class ColorChannelSprite;
class ColorSetupDelegate;
class LevelSettingsObject;
class cocos2d;

class GJColorSetupLayer : public FLAlertLayer, public ColorSelectDelegate, public FLAlertLayerProtocol {
public:
    virtual ~GJColorSetupLayer();
    virtual void keyBackClicked();
    virtual void colorSelectClosed(cocos2d::CCNode* popup);

    static GJColorSetupLayer* create(LevelSettingsObject* object);

    bool init(LevelSettingsObject* object);
    void onClose(cocos2d::CCObject* sender);
    void onColor(cocos2d::CCObject* sender);
    void onPage(cocos2d::CCObject* sender);
    void showPage(int page);
    void updateSpriteColor(ColorChannelSprite* sprite, cocos2d::CCLabelBMFont* label, int id);
    void updateSpriteColors();

    LevelSettingsObject* m_settingsObject;
    cocos2d::CCDictionary* m_unk290;
    cocos2d::CCDictionary* m_unk298;
    cocos2d::CCArray* m_colorLabels;
    cocos2d::CCArray* m_colorSprites;
    int m_page;
    int m_colorsPerPage;
    int m_totalPages;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    bool m_closeOnSelect;
    ColorSetupDelegate* m_delegate;
    int m_colorID;

};
