#pragma once

class CCTextInputNode;
class EmoteDef;
class EmotePickerPopup;
class GJMessageThread;
class ThreadViewPopup;
class cocos2d;

class ThreadDescPopup : public SetupTriggerPopup, public ThreadMessageDelegate, public EmotePickerDelegate {
public:
    virtual ~ThreadDescPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode*);
    virtual void emotePicked(EmotePickerPopup*, EmoteDef const*);
    virtual void emotePickerClosed(EmotePickerPopup*);
    virtual void threadUpdateFinished(int, int);
    virtual void threadUpdateFailed(int, int);
    virtual void threadMessagesFinished(int, cocos2d::CCArray*, bool);
    virtual void threadMessagesFailed(int);
    virtual void threadSearchFinished(int, cocos2d::CCArray*, bool);
    virtual void threadSearchFailed(int);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();

    static ThreadDescPopup* create(GJMessageThread*, ThreadViewPopup*);

    bool buildUI(bool, std::string const&, std::string const&);
    void /* TodoReturn */ finishSave(char const*);
    void /* TodoReturn */ handBackSlot();
    bool init(GJMessageThread*, ThreadViewPopup*);
    void onEmotes(cocos2d::CCObject* sender);
    void onPickIcon(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshIconSlot();
    void /* TodoReturn */ sendDescription();

};
