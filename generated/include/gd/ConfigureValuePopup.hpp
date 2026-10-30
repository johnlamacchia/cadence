#pragma once

class CCTextInputNode;
class ConfigureValuePopupDelegate;
class Slider;
class cocos2d;

class ConfigureValuePopup : public FLAlertLayer {
public:
    virtual ~ConfigureValuePopup();
    virtual void keyBackClicked();
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static ConfigureValuePopup* create(ConfigureValuePopupDelegate* delegate, float value, float minimum, float maximum, std::string title, std::string description, int type);

    void addInfoAlertButton(std::string title, std::string description);
    bool init(ConfigureValuePopupDelegate* delegate, float value, float minimum, float maximum, std::string title, std::string description, int type);
    void onArrow(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateTextInputLabel();

    int m_type;
    float m_value;
    float m_minimum;
    float m_maximum;
    bool m_enableDelegate;
    bool m_disableTextDelegate;
    ConfigureValuePopupDelegate* m_delegate;
    CCTextInputNode* m_input;
    Slider* m_slider;

};
