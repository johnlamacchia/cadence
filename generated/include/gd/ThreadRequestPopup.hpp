#pragma once

class GJMessageThread;
class InvitesPopup;
class cocos2d;

class ThreadRequestPopup : public FLAlertLayer {
public:
    virtual ~ThreadRequestPopup();
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
    virtual void keyBackClicked();

    static ThreadRequestPopup* create(GJMessageThread*, InvitesPopup*);

    bool init(GJMessageThread*, InvitesPopup*);
    void onAccept(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onIgnore(cocos2d::CCObject* sender);
    void onReport(cocos2d::CCObject* sender);
    void onViewMembers(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);

};
