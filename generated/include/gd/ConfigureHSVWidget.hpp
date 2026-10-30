#pragma once

class CCTextInputNode;
class GameObject;
class HSVWidgetDelegate;
class Slider;
class cocos2d;

class ConfigureHSVWidget : public cocos2d::CCNode, public TextInputDelegate {
public:
    virtual ~ConfigureHSVWidget();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);

    static ConfigureHSVWidget* create(cocos2d::ccHSVValue hsv, bool noBackground, bool addInputs);
    static cocos2d::ccHSVValue getHSV(GameObject* object, cocos2d::CCArray* objects, int mode);

    void createTextInputNode(cocos2d::CCPoint position, int tag);
    void /* TodoReturn */ hostPopup();
    bool init(cocos2d::ccHSVValue hsv, bool noBackground, bool addInputs);
    void onClose();
    void onResetHSV(cocos2d::CCObject* sender);
    void onToggleSConst(cocos2d::CCObject* sender);
    void onToggleVConst(cocos2d::CCObject* sender);
    void setBaseColor(cocos2d::ccColor3B);
    void sliderChanged(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateColorPreview();
    void updateLabels();
    void updateSliders();

    cocos2d::CCLabelBMFont* m_hueLabel;
    cocos2d::CCLabelBMFont* m_saturationLabel;
    cocos2d::CCLabelBMFont* m_brightnessLabel;
    Slider* m_hueSlider;
    Slider* m_saturationSlider;
    Slider* m_brightnessSlider;
    cocos2d::ccHSVValue m_hsv;
    bool m_mixed;
    HSVWidgetDelegate* m_delegate;
    bool m_addInputs;
    bool m_updating;
    cocos2d::CCDictionary* m_inputs;
    cocos2d::ccColor3B m_baseColor;
    cocos2d::CCSprite* m_basePreview;
    cocos2d::CCSprite* m_hsvPreview;
    bool m_hasHSVPreview;

};
