#pragma once

class EditorSoundsLayer;
class GJEditorSoundConfig;
class SetTextPopup;
class TableView;
class TableViewCell;
class cocos2d;

class EditorSoundBrowserLayer : public SetupTriggerPopup, public EditorSoundListDelegate, public EditorSoundConfigCellDelegate, public TableViewCellDelegate, public SetTextPopupDelegate {
public:
    virtual ~EditorSoundBrowserLayer();
    virtual void registerWithTouchDispatcher();
    virtual void rebuildContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void setTextPopupClosed(SetTextPopup*, std::string);
    virtual void loadEditorSoundsFinished(cocos2d::CCArray*, char const*);
    virtual void loadEditorSoundsFailed(char const*);
    virtual void onConfigViewClicked(GJEditorSoundConfig*);
    virtual bool cellPerformedAction(TableViewCell*, int, CellAction, cocos2d::CCNode*);
    virtual int getSelectedCellIdx();

    static EditorSoundBrowserLayer* create(std::string, EditorSoundsLayer*);

    void /* TodoReturn */ clearCells();
    bool init(std::string, EditorSoundsLayer*);
    void /* TodoReturn */ layoutCells(cocos2d::CCArray*, char const*);
    void /* TodoReturn */ loadCurrentPage(bool, bool);
    void /* TodoReturn */ onConfigCopiedToMyLists(GJEditorSoundConfig*);
    void /* TodoReturn */ onConfigListChanged();
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void onRefresh(cocos2d::CCObject* sender);
    void onSaveActive(cocos2d::CCObject* sender);
    void onSelectMode(cocos2d::CCObject* sender);
    void setMode(EditorSoundBrowseMode);
    void /* TodoReturn */ showLoading(bool);
    void /* TodoReturn */ updateFooterButtonVisibility();
    void /* TodoReturn */ updatePageLabel();

};
