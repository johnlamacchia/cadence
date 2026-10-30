#pragma once

class cocos2d;

class RowPressItem : public CCMenuItemSpriteExtra {
public:
    virtual ~RowPressItem();
    virtual void selected();
    virtual void unselected();

    static RowPressItem* create(cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);

};
