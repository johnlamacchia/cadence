#pragma once

class cocos2d;

class PromoInterstitial : public FLAlertLayer {
public:
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void show();

    static PromoInterstitial* create(bool fullVersion);

    bool init(bool fullVersion);
    void onClick(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void setup();

    bool m_fullVersion;
    cocos2d::CCMenu* m_closeMenu;
    bool m_promoTouched;
    std::string m_marketLink;
    bool m_showAdBanner;

};
