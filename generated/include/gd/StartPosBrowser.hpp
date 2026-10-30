#pragma once

class OptionsObject;
class TableView;
class TableViewCell;
class cocos2d;

class StartPosBrowser : public FLAlertLayer, public TableViewCellDelegate, public OptionsObjectDelegate {
public:
    virtual ~StartPosBrowser();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual bool cellPerformedAction(TableViewCell*, int, CellAction, cocos2d::CCNode*);
    virtual int getSelectedCellIdx();
    virtual void stateChanged(OptionsObject*);

    static StartPosBrowser* create(cocos2d::CCArray*, int);

    bool init(cocos2d::CCArray*, int);
    void onClose(cocos2d::CCObject* sender);
    void setupList();
    void updatePageLabel();

};
