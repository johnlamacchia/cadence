#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class ShardsPage : public FLAlertLayer {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();

    static ShardsPage* create();

    ShardsPage();
    ~ShardsPage();
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2);
    void goToPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onIconInfo(cocos2d::CCObject* sender);
    void onSwitchPage(cocos2d::CCObject* sender);

    cocos2d::CCDictionary* m_pages;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    cocos2d::CCSprite* m_tierSprite;
    int m_page;

};
