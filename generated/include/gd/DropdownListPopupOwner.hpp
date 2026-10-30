#pragma once

class Dropdown;
class DropdownListPopup;

class DropdownListPopupOwner {
public:
    virtual void dropdownListPopupClosed(DropdownListPopup*);

};
