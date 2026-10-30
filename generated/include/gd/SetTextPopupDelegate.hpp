#pragma once

class SetTextPopup;

class SetTextPopupDelegate {
public:
    virtual void setTextPopupClosed(SetTextPopup* popup, std::string text);

};
