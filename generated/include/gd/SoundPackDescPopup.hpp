#pragma once

class CCTextInputNode;
class EmoteDef;
class EmotePickerPopup;
class SoundPackDescDelegate;
class cocos2d;

class SoundPackDescPopup : public SetupTriggerPopup, public EmotePickerDelegate {
public:
    virtual ~SoundPackDescPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode*);
    virtual void emotePicked(EmotePickerPopup*, EmoteDef const*);
    virtual void emotePickerClosed(EmotePickerPopup*);

    static SoundPackDescPopup* create(std::string const&, std::string const&, SoundPackDescDelegate*);

    bool init(std::string const&, std::string const&, SoundPackDescDelegate*);
    void onEmotes(cocos2d::CCObject* sender);
    void onPickIcon(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshIconSlot();

};
