#pragma once

class ConfigureHSVWidget;
class HSVWidgetDelegate;
class cocos2d;

class HSVWidgetPopup : public FLAlertLayer {
public:
    virtual ~HSVWidgetPopup();
    virtual void keyBackClicked();

    static HSVWidgetPopup* create(cocos2d::ccHSVValue, cocos2d::ccColor3B, HSVWidgetDelegate*, std::string);

    bool init(cocos2d::ccHSVValue, cocos2d::ccColor3B, HSVWidgetDelegate*, std::string);
    void onClose(cocos2d::CCObject* sender);

    ConfigureHSVWidget* m_widget;
    HSVWidgetDelegate* m_delegate;

};
