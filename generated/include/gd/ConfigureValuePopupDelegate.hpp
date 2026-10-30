#pragma once

class ConfigureValuePopup;

class ConfigureValuePopupDelegate {
public:
    virtual void valuePopupClosed(ConfigureValuePopup* popup, float value);

};
