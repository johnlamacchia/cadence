#pragma once

class FLAlertLayer;
class GJMessageThread;
class ThreadViewPopup;
class cocos2d;

class ThreadOptionsPopup : public SetupTriggerPopup, public ThreadMessageDelegate, public FLAlertLayerProtocol {
public:
    virtual ~ThreadOptionsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void threadMessagesFinished(int, cocos2d::CCArray*, bool);
    virtual void threadMessagesFailed(int);
    virtual void threadSearchFinished(int, cocos2d::CCArray*, bool);
    virtual void threadSearchFailed(int);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();

    static ThreadOptionsPopup* create(GJMessageThread*, ThreadViewPopup*, bool);

    void /* TodoReturn */ handBackSlot();
    bool init(GJMessageThread*, ThreadViewPopup*, bool);
    void onCancelRequest(cocos2d::CCObject* sender);
    void onDismissRemoved(cocos2d::CCObject* sender);
    void onHide(cocos2d::CCObject* sender);
    void onLeave(cocos2d::CCObject* sender);
    void onMembers(cocos2d::CCObject* sender);
    void onModLock(cocos2d::CCObject* sender);
    void onMuteRow(cocos2d::CCObject* sender);
    void onPinRow(cocos2d::CCObject* sender);
    void onRename(cocos2d::CCObject* sender);
    void onReport(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);

};
