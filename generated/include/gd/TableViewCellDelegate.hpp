#pragma once

class TableView;
class TableViewCell;
class cocos2d;

class TableViewCellDelegate {
public:
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual int getSelectedCellIdx();
    virtual void /* TodoReturn */ getSelectedCellCustomIdx();
    virtual void /* TodoReturn */ allowsCustomSFXActions();
    virtual bool isCustomIDSelected(int);
    virtual void /* TodoReturn */ inCustomSFXTree();
    virtual void /* TodoReturn */ showsSFXInfoButton();
    virtual bool shouldSnapToSelected();
    virtual int getCellDelegateType();

};
