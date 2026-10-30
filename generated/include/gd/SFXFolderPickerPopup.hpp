#pragma once

class SFXFolderPickerDelegate;
class SetTextPopup;
class cocos2d;

class SFXFolderPickerPopup : public FLAlertLayer, public SetTextPopupDelegate {
public:
    virtual ~SFXFolderPickerPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void setTextPopupClosed(SetTextPopup*, std::string);

    static SFXFolderPickerPopup* create(SFXFolderPickerMode, std::string const&, std::set<int> const&, SFXFolderPickerDelegate*, int);

    bool init(SFXFolderPickerMode, std::string const&, std::set<int> const&, SFXFolderPickerDelegate*, int);
    void onClose(cocos2d::CCObject* sender);
    void onConfirm(cocos2d::CCObject* sender);
    void onNewFolder(cocos2d::CCObject* sender);
    void onPickFolder(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void onSelectAll(cocos2d::CCObject* sender);
    void /* TodoReturn */ pickFolder(int);
    void /* TodoReturn */ rebuildList();
    void /* TodoReturn */ refreshList();
    void /* TodoReturn */ updateActionBtn();
    void /* TodoReturn */ updateShareHint();

};
