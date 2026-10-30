#pragma once

class CCTextInputNode;
class FLAlertLayer;
class cocos2d;

class AutoBackupPopup : public PagedListPopup, public FLAlertLayerProtocol, public AutoBackupOptionsDelegate {
public:
    virtual ~AutoBackupPopup();
    virtual int requestPage(int);
    virtual void /* TodoReturn */ clearPageContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void backupsChanged();
    virtual void textChanged(CCTextInputNode*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static AutoBackupPopup* create(int);

    void /* TodoReturn */ assignRevs();
    void /* TodoReturn */ backupDirectory();
    void /* TodoReturn */ buildRows();
    bool init(int);
    void onClearSearch(cocos2d::CCObject* sender);
    void onDelete(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void onRestore(cocos2d::CCObject* sender);
    void onSelectAll(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void onToggleRow(cocos2d::CCObject* sender);
    void /* TodoReturn */ pruneBackups(std::string const&, int, std::string const&, bool);
    void /* TodoReturn */ reloadFiles();
    void /* TodoReturn */ selectedCount();
    void /* TodoReturn */ updateSelectUI();
    void /* TodoReturn */ updateSummary();

};
