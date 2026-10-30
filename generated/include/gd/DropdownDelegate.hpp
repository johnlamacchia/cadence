#pragma once

class Dropdown;

class DropdownDelegate {
public:
    virtual void dropdownSelectionChanged(Dropdown*, int);

};
