#pragma once

class KeybindSet;
class KeybindSource;
class LevelTouchSlot;
class UIButtonConfig;
class cocos2d;

class LevelControlConfig : public cocos2d::CCObject {
public:
    virtual ~LevelControlConfig();

    static LevelControlConfig* create();
    static void bindGlobalBinds(KeybindSource* target, bool controller);
    static void bindDefaultController(KeybindSource* target);
    static void bindDefaultKeyboard(KeybindSource* target);
    static LevelControlConfig* createFromB64(std::string const& data);
    static LevelControlConfig* createFromGlobal(bool dualMode);

    bool decode(std::string const& data);
    std::string encode();
    std::string encodeB64();
    bool init();
    void keepClasses(int classes);
    int presentClasses();
    UIButtonConfig resolveSlot(LevelTouchSlot const& slot, cocos2d::CCSize const& winSize);
    void seedBase();
    void seedDefaults();
    LevelTouchSlot slotFromConfig(UIButtonConfig const& config, cocos2d::CCSize const& winSize, int anchorX, int anchorY, bool scaleX, bool scaleY);

    int m_version;
    cocos2d::CCSize m_canvasSize;
    int m_jumpL;
    KeybindSet m_keyboardBinds;
    KeybindSet m_controllerBinds;
    std::vector<LevelTouchSlot> m_touchSlots;

};
