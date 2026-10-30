#pragma once

class ListButtonBar;

class ListButtonBarDelegate {
public:
    virtual void listButtonBarSwitchedPage(ListButtonBar* bar, int page);

};
