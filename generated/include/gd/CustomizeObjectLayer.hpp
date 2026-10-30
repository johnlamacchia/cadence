#pragma once

class ButtonSprite;
class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class CCTextInputNode;
class ColorChannelSprite;
class GameObject;
class HSVWidgetPopup;
class Slider;
class cocos2d;

class CustomizeObjectLayer : public FLAlertLayer, public HSVWidgetDelegate, public ColorSelectDelegate, public ColorSetupDelegate {
public:
    virtual ~CustomizeObjectLayer();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value);
    virtual void colorSelectClosed(cocos2d::CCNode* popup);
    virtual void colorSetupClosed(int id);

    static CustomizeObjectLayer* create(GameObject* object, cocos2d::CCArray* objects);

    CCMenuItemToggler* createToggleButton(std::string text, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    void determineStartValues();
    int getActiveMode(bool ignoreDefault);
    ButtonSprite* getButtonByTag(int tag);
    cocos2d::ccHSVValue getHSV();
    void highlightSelected(ButtonSprite* sprite);
    bool init(GameObject* object, cocos2d::CCArray* objects);
    void onBreakApart(cocos2d::CCObject* sender);
    void onBrowse(cocos2d::CCObject* sender);
    void onClear(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);
    void onEditColor(cocos2d::CCObject* sender);
    void onHSV(cocos2d::CCObject* sender);
    void onLiveEdit(cocos2d::CCObject* sender);
    void onNextColorChannel(cocos2d::CCObject* sender);
    void onPaste(cocos2d::CCObject* sender);
    void onSelectColor(cocos2d::CCObject* sender);
    void onSelectMode(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);
    void onUpdateCustomColor(cocos2d::CCObject* sender);
    void recreateLayer();
    void sliderChanged(cocos2d::CCObject* sender);
    void toggleVisible();
    void updateChannelLabel(int channel);
    void updateColorSprite();
    void updateCurrentSelection();
    void updateCustomColorLabels();
    void updateHSVButtons();
    void updateKerningLabel();
    void updateSelected(int id);

    GameObject* m_targetObject;
    cocos2d::CCArray* m_targetObjects;
    cocos2d::CCArray* m_colorButtons;
    cocos2d::CCArray* m_colorTabNodes;
    cocos2d::CCArray* m_textTabNodes;
    cocos2d::CCArray* m_unkArray;
    cocos2d::CCArray* m_detailTabNodes;
    int m_selectedMode;
    int m_customColorChannel;
    bool m_hasBaseColor;
    bool m_hasDetailColor;
    bool m_glowDisabled;
    CCMenuItemSpriteExtra* m_baseButton;
    CCMenuItemSpriteExtra* m_detailButton;
    CCMenuItemSpriteExtra* m_textButton;
    CCMenuItemSpriteExtra* m_baseColorHSV;
    CCMenuItemSpriteExtra* m_detailColorHSV;
    cocos2d::CCLabelBMFont* m_titleLabel;
    cocos2d::CCLabelBMFont* m_selectedColorLabel;
    CCTextInputNode* m_customColorInput;
    CCTextInputNode* m_textInput;
    int m_kerningAmount;
    cocos2d::CCLabelBMFont* m_kerningLabel;
    Slider* m_kerningSlider;
    ButtonSprite* m_customColorButtonSprite;
    CCMenuItemSpriteExtra* m_customColorButton;
    CCMenuItemSpriteExtra* m_arrowDown;
    CCMenuItemSpriteExtra* m_arrowUp;
    cocos2d::extension::CCScale9Sprite* m_customColorInputBG;
    ColorChannelSprite* m_colorSprite;
    CCMenuItemSpriteExtra* m_colorSpriteButton;
    CCMenuItemSpriteExtra* m_liveSelectButton;
    bool m_showTextInput;
    bool m_customColorSelected;
    bool m_disableTextDelegate;

};
