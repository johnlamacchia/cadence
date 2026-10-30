#pragma once

class cocos2d;

class GJPromoPopup : public FLAlertLayer {
public:
    virtual ~GJPromoPopup();
    virtual void onExit();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();

    static GJPromoPopup* create(std::string filename);

    GJPromoPopup();
    bool init(std::string filename);
    void onClose(cocos2d::CCObject* sender);

    std::string m_promoFrame;

};
