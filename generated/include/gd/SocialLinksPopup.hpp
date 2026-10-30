#pragma once

class GJAccountSettingsLayer;
class cocos2d;

class SocialLinksPopup : public FLAlertLayer {
public:
    virtual ~SocialLinksPopup();
    virtual void keyBackClicked();

    static SocialLinksPopup* create(GJAccountSettingsLayer*);

    bool init(GJAccountSettingsLayer*);
    void onClose(cocos2d::CCObject* sender);
    void onOK(cocos2d::CCObject* sender);

};
