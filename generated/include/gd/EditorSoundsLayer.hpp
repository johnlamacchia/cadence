#pragma once

class CCTextInputNode;
class EditorUI;
class cocos2d;

class EditorSoundsLayer : public TabbedPopupLayer {
public:
    virtual ~EditorSoundsLayer();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void rebuildContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);
    virtual void showTab(int tab);

    static EditorSoundsLayer* create(EditorUI*);

    void /* TodoReturn */ buildContentForTab(int);
    bool init(EditorUI*);
    void onActionRow(cocos2d::CCObject* sender);
    void onBrowse(cocos2d::CCObject* sender);
    void onClearSearch(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);
    void /* TodoReturn */ setupTabs();

};
