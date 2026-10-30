#pragma once

class cocos2d;

class PagedListPopup : public FLAlertLayer {
public:
    virtual ~PagedListPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void show();
    virtual void /* TodoReturn */ clearPageContent();
    virtual void onClose(cocos2d::CCObject* sender);

    void /* TodoReturn */ clearStatus();
    void /* TodoReturn */ createScrollArea(float, float);
    void /* TodoReturn */ createScrollView(float, float, cocos2d::CCPoint);
    void /* TodoReturn */ fadeLoading();
    void /* TodoReturn */ initFrame(float, float);
    void /* TodoReturn */ initPaged(char const*, std::string const&, float, float);
    void /* TodoReturn */ loadPage(int);
    void onNextPage(cocos2d::CCObject* sender);
    void onPrevPage(cocos2d::CCObject* sender);
    void /* TodoReturn */ retryPendingPage();
    void /* TodoReturn */ showLoading();
    void /* TodoReturn */ showStatus(char const*);
    void /* TodoReturn */ updatePageArrows(bool, bool);
    void /* TodoReturn */ updatePageUI(int, int, int);

    char _pad_0[112];

};
