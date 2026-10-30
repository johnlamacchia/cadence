#pragma once

class cocos2d;

class TOSPopup : public FLAlertLayer {
public:
    virtual ~TOSPopup();
    virtual bool init();
    virtual void keyBackClicked();

    static TOSPopup* create();

    TOSPopup();
    void onClose(cocos2d::CCObject* sender);
    void onPrivacy(cocos2d::CCObject* sender);
    void onTOS(cocos2d::CCObject* sender);

};
