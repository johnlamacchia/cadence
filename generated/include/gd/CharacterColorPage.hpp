#pragma once

class CCMenuItemToggler;
class CharacterColorDelegate;
class cocos2d;

class CharacterColorPage : public FLAlertLayer {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();

    static CharacterColorPage* create();

    CharacterColorPage();
    ~CharacterColorPage();
    int activeColorForMode(int mode);
    bool checkColor(int id, UnlockType type);
    int colorForIndex(int index);
    void createColorMenu();
    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    cocos2d::CCPoint offsetForIndex(int index);
    void onClose(cocos2d::CCObject* sender);
    void onMode(cocos2d::CCObject* sender);
    void onPlayerColor(cocos2d::CCObject* sender);
    void toggleGlow(cocos2d::CCObject* sender);
    void toggleGlowItems(bool visible);
    void toggleShip(cocos2d::CCObject* sender);
    void updateColorMode(int mode);
    void updateIconColors();

    int m_colorMode;
    float m_height;
    float m_width;
    cocos2d::CCArray* m_playerObjects;
    cocos2d::CCArray* m_modeButtons;
    cocos2d::CCDictionary* m_colorButtons;
    cocos2d::CCArray* m_cursors;
    CharacterColorDelegate* m_delegate;
    CCMenuItemToggler* m_glowToggler;
    cocos2d::CCLabelBMFont* m_glowLabel;

};
