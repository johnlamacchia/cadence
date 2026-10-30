#pragma once

class cocos2d;

class GJPathRewardPopup : public FLAlertLayer {
public:
    virtual ~GJPathRewardPopup();
    virtual void keyBackClicked();

    static GJPathRewardPopup* create(int path);

    GJPathRewardPopup();
    void closePopup();
    bool init(int path);
    void onClaim(cocos2d::CCObject* sender);

    int m_pathNumber;

};
