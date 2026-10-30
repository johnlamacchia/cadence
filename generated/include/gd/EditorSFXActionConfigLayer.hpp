#pragma once

class EditorSoundsLayer;
class SFXBrowser;
class cocos2d;

class EditorSFXActionConfigLayer : public SetupTriggerPopup, public SFXBrowserDelegate {
public:
    virtual ~EditorSFXActionConfigLayer();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void rebuildContent();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);
    virtual float getValue(int);
    virtual void sfxBrowserClosed(SFXBrowser*);

    static EditorSFXActionConfigLayer* create(int, EditorSoundsLayer*);

    bool init(int, EditorSoundsLayer*);
    void onAddSound(cocos2d::CCObject* sender);
    void onDeleteSound(cocos2d::CCObject* sender);
    void onEditSound(cocos2d::CCObject* sender);
    void onOpenSettings(cocos2d::CCObject* sender);
    void onPreviewSound(cocos2d::CCObject* sender);
    void onToggleMode(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshModeButtons();
    void /* TodoReturn */ refreshRows();
    void /* TodoReturn */ refreshRowsAddedDeferred(float);

};
