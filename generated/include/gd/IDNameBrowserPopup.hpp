#pragma once

class CCTextInputNode;
class IDNameBrowserDelegate;
class cocos2d;

class IDNameBrowserPopup : public FLAlertLayer, public VirtualRowSource {
public:
    virtual ~IDNameBrowserPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void show();
    virtual void textChanged(CCTextInputNode*);
    virtual void textInputClosed(CCTextInputNode*);
    virtual int virtualRowCount();
    virtual int virtualRowHeight(int);
    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*);

    static IDNameBrowserPopup* create(int, int, IDNameBrowserDelegate*);

    void /* TodoReturn */ getFormID();
    bool init(int, int, IDNameBrowserDelegate*);
    void onClose(cocos2d::CCObject* sender);
    void onDeleteRow(cocos2d::CCObject* sender);
    void onEditRow(cocos2d::CCObject* sender);
    void onFormAdd(cocos2d::CCObject* sender);
    void onFormArrowLeft(cocos2d::CCObject* sender);
    void onFormArrowRight(cocos2d::CCObject* sender);
    void onFormNextFree(cocos2d::CCObject* sender);
    void onSelectRow(cocos2d::CCObject* sender);
    void onToggleSort(cocos2d::CCObject* sender);
    void /* TodoReturn */ rebuildList();
    void setFormID(int);
    void /* TodoReturn */ updateConflictLabel();
    void /* TodoReturn */ updateSortButtonLabel();

};
