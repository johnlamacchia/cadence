#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class CommunityCreditsPage : public FLAlertLayer {
public:
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();

    static CommunityCreditsPage* create();

    CommunityCreditsPage();
    ~CommunityCreditsPage();
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2);
    void goToPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onSwitchPage(cocos2d::CCObject* sender);

    cocos2d::CCDictionary* m_pageObjects;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    int m_page;

};
