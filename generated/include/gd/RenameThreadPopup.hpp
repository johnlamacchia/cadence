#pragma once

class GJMessageThread;
class ThreadViewPopup;
class cocos2d;

class RenameThreadPopup : public SetupTriggerPopup, public ThreadMessageDelegate {
public:
    virtual ~RenameThreadPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void threadMessagesFinished(int, cocos2d::CCArray*, bool);
    virtual void threadMessagesFailed(int);
    virtual void threadSearchFinished(int, cocos2d::CCArray*, bool);
    virtual void threadSearchFailed(int);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();

    static RenameThreadPopup* create(GJMessageThread*, ThreadViewPopup*);

    void /* TodoReturn */ handBackSlot();
    bool init(GJMessageThread*, ThreadViewPopup*);
    void onSave(cocos2d::CCObject* sender);

};
