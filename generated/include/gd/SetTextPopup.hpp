#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class EmotePickerPopup;
class SetTextPopupDelegate;
class cocos2d;

class SetTextPopup : public FLAlertLayer, public EmotePickerDelegate {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void emotePickerClosed(EmotePickerPopup*);

    static SetTextPopup* create(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int);

    SetTextPopup();
    ~SetTextPopup();
    void /* TodoReturn */ addInfoButton(std::string const&, std::string const&);
    void /* TodoReturn */ addInfoLabel(std::string const&);
    void /* TodoReturn */ addPathLabel(std::string const&);
    bool init(std::string, std::string, int, std::string, std::string, bool, float, bool, int, int);
    void onCancel(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onEmotes(cocos2d::CCObject* sender);
    void onResetValue(cocos2d::CCObject* sender);
    void updateTextInputLabel();

    CCTextInputNode* m_input;
    bool m_disableDelegate;
    bool m_cancelled;
    float m_unk2ac;
    float m_unk2b0;
    std::string m_value;
    int m_unk2d8;
    int m_unk2dc;
    CCMenuItemSpriteExtra* m_emoteButton;
    EmotePickerPopup* m_emotePicker;
    SetTextPopupDelegate* m_delegate;

};
