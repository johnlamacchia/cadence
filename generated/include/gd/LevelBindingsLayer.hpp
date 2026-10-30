#pragma once

class KeybindSet;
class KeybindSettingsEntry;
class cocos2d;

class LevelBindingsLayer : public KeybindingsLayer {
public:
    virtual ~LevelBindingsLayer();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void setupTabs();
    virtual std::vector<int> actionsToShow(int tab);
    virtual void openBindingEditor(int action, int chord);
    virtual bool showControllerChords();
    virtual std::vector<KeybindSettingsEntry> settingsEntries();
    virtual bool crossCheckGlobalConflicts();

    static LevelBindingsLayer* create(KeybindSet* keybindSet, bool controller);

    bool init(KeybindSet* keybindSet, bool controller);
    void onApplyGlobal(cocos2d::CCObject* sender);
    void onClear(cocos2d::CCObject* sender);
    void onCopy(cocos2d::CCObject* sender);
    void onPaste(cocos2d::CCObject* sender);
    void onReset(cocos2d::CCObject* sender);

    KeybindSet* m_keybindSet;
    bool m_controller;
    bool m_seededFromGlobal;

};
