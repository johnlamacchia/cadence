#pragma once

class GJUserScore;
class cocos2d;

class MyStuffPopup : public FLAlertLayer {
public:
    virtual ~MyStuffPopup();
    virtual void keyBackClicked();

    static MyStuffPopup* create(GJUserScore*);

    bool init(GJUserScore*);
    void onClose(cocos2d::CCObject* sender);
    void onSocialPage(cocos2d::CCObject* sender);

};
