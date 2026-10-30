#pragma once

class LevelEditorLayer;
class SelectFontLayer;
class TableView;
class TableViewCell;
class cocos2d;

class SelectFontLayer2 : public FLAlertLayer, public TableViewCellDelegate {
public:
    virtual ~SelectFontLayer2();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual bool cellPerformedAction(TableViewCell*, int, CellAction, cocos2d::CCNode*);
    virtual int getSelectedCellIdx();

    static SelectFontLayer2* create(LevelEditorLayer*);

    bool init(LevelEditorLayer*);
    void /* TodoReturn */ loadPreviewSheet();
    void onClose(cocos2d::CCObject* sender);
    void /* TodoReturn */ onSelectFont(int);
    void /* TodoReturn */ unloadPreviewSheet();

};
