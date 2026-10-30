#pragma once

class Dropdown;
class DropdownListRow;
class DropdownListStyle;
class cocos2d;

class DropdownListPopup : public FLAlertLayer {
public:
    virtual ~DropdownListPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();

    static DropdownListPopup* create(std::vector<DropdownListRow> const&, cocos2d::CCPoint, DropdownListStyle const&, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);

    void /* TodoReturn */ buildRows();
    void /* TodoReturn */ closeSelf();
    bool init(std::vector<DropdownListRow> const&, cocos2d::CCPoint, DropdownListStyle const&, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
    void onRowSelected(cocos2d::CCObject* sender);

};
