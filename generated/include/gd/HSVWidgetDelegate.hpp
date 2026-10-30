#pragma once

class ConfigureHSVWidget;
class HSVWidgetPopup;
class cocos2d;

class HSVWidgetDelegate {
public:
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value);
    virtual void hsvChanged(ConfigureHSVWidget* widget);

};
