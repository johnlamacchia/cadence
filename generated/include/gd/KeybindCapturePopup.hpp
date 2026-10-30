#pragma once

class ButtonSprite;
class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class ControllerInputNode;
class KeybindSource;
class KeybindingsLayer;
class VirtualKeyboardNode;
class cocos2d;

class KeybindCapturePopup : public SetupTriggerPopup, public InputCaptureDelegate {
public:
    virtual ~KeybindCapturePopup();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void inputKeySelected(int chord);
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);

    static KeybindCapturePopup* create(KeybindSource* source, int action, int chord, KeybindingsLayer* layer, KeybindCaptureMode mode);

    void fadeButtonSprite(ButtonSprite* sprite, bool enabled);
    bool init(KeybindSource* source, int action, int chord, KeybindingsLayer* layer, KeybindCaptureMode mode);
    void onRemove(cocos2d::CCObject* sender);
    void onSet(cocos2d::CCObject* sender);
    void onToggleController(cocos2d::CCObject* sender);
    void updateButtonStates();
    void updateChordDisplay();
    void updateConflictLabel();

    KeybindSource* m_keybindSource;
    KeybindingsLayer* m_keybindingsLayer;
    int m_action;
    int m_originalChord;
    int m_chord;
    bool m_isNewBind;
    bool m_awaitingConfirm;
    bool m_controllerMode;
    bool m_isLevelSet;
    bool m_isBlocked;
    VirtualKeyboardNode* m_virtualKeyboard;
    ControllerInputNode* m_controllerInput;
    CCMenuItemToggler* m_controllerToggler;
    cocos2d::CCLabelBMFont* m_chordLabel;
    cocos2d::CCLabelBMFont* m_promptLabel;
    cocos2d::CCLabelBMFont* m_conflictLabel;
    CCMenuItemSpriteExtra* m_setButton;
    CCMenuItemSpriteExtra* m_removeButton;
    ButtonSprite* m_setSprite;
    ButtonSprite* m_removeSprite;

};
