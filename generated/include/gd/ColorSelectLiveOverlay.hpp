#pragma once

class CCMenuItemToggler;
class CCTextInputNode;
class ColorAction;
class ColorSelectDelegate;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class ColorSelectLiveOverlay : public FLAlertLayer {
public:
    virtual ~ColorSelectLiveOverlay();
    virtual void keyBackClicked();
    virtual void show();

    static ColorSelectLiveOverlay* create(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object);
    static ColorSelectLiveOverlay* createWithActions(ColorAction* baseAction, ColorAction* detailAction);
    static ColorSelectLiveOverlay* createWithObject(EffectGameObject* object);

    void closeColorSelect(cocos2d::CCObject* sender);
    void colorValueChanged(cocos2d::ccColor3B color);
    CCMenuItemToggler* createToggleButton(std::string label, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position);
    void determineStartValues();
    cocos2d::ccColor3B getColorValue();
    bool init(ColorAction* baseAction, ColorAction* detailAction, EffectGameObject* object);
    void onSelectTab(cocos2d::CCObject* sender);
    void selectColor(cocos2d::ccColor3B color);
    void sliderChanged(cocos2d::CCObject* sender);
    void textChanged(CCTextInputNode* node);
    void textInputClosed(CCTextInputNode* node);
    void toggleControls(bool visible);
    void updateColorLabel();
    void updateColorValue();
    void updateOpacity();

    EffectGameObject* m_effectGameObject;
    cocos2d::CCArray* m_barSprites;
    cocos2d::CCArray* m_12buttons;
    ColorAction* m_baseColorAction;
    ColorAction* m_detailColorAction;
    cocos2d::ccColor3B m_mainColor;
    cocos2d::ccColor3B m_detailColor;
    cocos2d::ccColor3B m_targetColor;
    float m_unk2ac;
    int m_unk2b0;
    int m_unk2b4;
    float m_opacity;
    ColorSelectDelegate* m_delegate;
    bool m_disableTextDelegate;
    bool m_unk2c9;
    bool m_updateColor;
    Slider* m_hueSlider;
    Slider* m_saturationSlider;
    Slider* m_valueSlider;
    Slider* m_opacitySlider;
    cocos2d::CCSprite* m_colorSprite;
    cocos2d::CCSprite* m_oldColorSprite;
    cocos2d::CCSprite* m_saturationSprite;
    cocos2d::CCSprite* m_valueSprite;
    cocos2d::CCSprite* m_opacitySprite;
    double m_hue;
    double m_saturation;
    double m_value;
    cocos2d::CCLabelBMFont* m_colorLabel;
    bool m_detailColorSelected;
    bool m_closeButtonPressed;

};
