#pragma once

class CCTextInputNode;
class EmoteDef;
class EmotePickerPopup;
class GJGameLevel;
class GJLevelList;
class GJUserScore;
class cocos2d;

class NewThreadPopup : public FLAlertLayer, public ThreadMessageDelegate, public FriendPickerDelegate, public LevelPickerDelegate, public EmotePickerDelegate {
public:
    virtual ~NewThreadPopup();
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void show();
    virtual void emotePickerKeyDown(EmotePickerPopup*, cocos2d::enumKeyCodes, double);
    virtual void emotePickerClosed(EmotePickerPopup*);
    virtual void emotePicked(EmotePickerPopup*, EmoteDef const*);
    virtual void levelPicked(GJGameLevel*);
    virtual void listPicked(GJLevelList*);
    virtual void shareIDPicked(int, bool);
    virtual void friendPicked(GJUserScore*);
    virtual bool isRecipientSelected(int);
    virtual void /* TodoReturn */ selectedRecipientCount();
    virtual void /* TodoReturn */ selectedRecipients();
    virtual void textChanged(CCTextInputNode*);
    virtual void threadSendFinished(int, long long);
    virtual void threadSendFailed();

    static NewThreadPopup* create(int, std::string const&, std::string const&);

    void /* TodoReturn */ applyMode();
    void /* TodoReturn */ bodyCharLimit();
    void /* TodoReturn */ bodyEmoteCap();
    bool init(int, std::string const&, std::string const&);
    void /* TodoReturn */ insertShareToken(std::string const&);
    void onClose(cocos2d::CCObject* sender);
    void onEmotes(cocos2d::CCObject* sender);
    void onOpenPicker(cocos2d::CCObject* sender);
    void onPickIcon(cocos2d::CCObject* sender);
    void onSend(cocos2d::CCObject* sender);
    void onShareLevel(cocos2d::CCObject* sender);
    void onToggleMode(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshIconSlot();
    void setSendPending(bool);
    void /* TodoReturn */ updateCharCount(cocos2d::CCLabelBMFont*, int, int);
    void /* TodoReturn */ updateRecipientLabel();

};
