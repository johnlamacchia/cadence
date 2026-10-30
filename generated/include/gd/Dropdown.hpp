#pragma once

class DropdownDelegate;
class DropdownListPopup;
class FLAlertLayer;
class cocos2d;

class Dropdown : public cocos2d::CCNode, public DropdownListPopupOwner {
public:
    virtual ~Dropdown();
    virtual void setPosition(cocos2d::CCPoint const&);
    virtual void setVisible(bool);
    virtual void dropdownListPopupClosed(DropdownListPopup*);

    static Dropdown* create(FLAlertLayer* parent, cocos2d::CCMenu* menu, std::vector<std::string> const& items, int selectedIndex, float width, DropdownDelegate* delegate, int tag);

    void /* TodoReturn */ closeList();
    bool init(FLAlertLayer* parent, cocos2d::CCMenu* menu, std::vector<std::string> const& items, int selectedIndex, float width, DropdownDelegate* delegate, int tag);
    void onRowSelected(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    void /* TodoReturn */ openList();
    void /* TodoReturn */ refreshValueLabel();
    void setSelectedIndex(int);

};
