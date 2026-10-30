#pragma once

class FLAlertLayer;
class GJThreadMessage;
class ThreadViewPopup;
class cocos2d;

class ThreadMessageOptionsPopup : public SetupTriggerPopup, public FLAlertLayerProtocol {
public:
    virtual ~ThreadMessageOptionsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static ThreadMessageOptionsPopup* create(GJThreadMessage*, std::string const&, ThreadViewPopup*);

    bool init(GJThreadMessage*, std::string const&, ThreadViewPopup*);
    void onAction(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);

};
