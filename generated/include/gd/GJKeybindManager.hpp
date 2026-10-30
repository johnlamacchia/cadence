#pragma once

class DS_Dictionary;
class KeybindSource;
class cocos2d;

class GJKeybindManager : public cocos2d::CCKeyboardActionManager {
public:
    virtual ~GJKeybindManager();
    virtual std::vector<int> actionsForKey(cocos2d::enumKeyCodes key, bool ctrl, bool alt, bool shift);
    virtual bool handleGlobalAction(int action, bool down);

    static GJKeybindManager* sharedState();
    static char const* actionToString(int action);
    static std::string chordToString(int chord);
    static int encodeChord(cocos2d::enumKeyCodes key, bool ctrl, bool alt, bool shift);
    static bool groupsConflict(int groupA, int groupB);
    static bool handleGlobalAudioAction(int action, bool down);
    static bool isControllerChord(int chord);
    static bool isExclusive(int action);
    static bool isKnownAction(int action);
    static bool isMovementAction(int action);

    std::vector<int> actionsForChord(int chord);
    void applyImportBlob(std::string const& blob, int version);
    void applyMovementOverride(KeybindSource* keyboardSource, KeybindSource* controllerSource);
    void applyNewDefaults(int version);
    void bindChord(int chord, int action);
    void bindKey(cocos2d::enumKeyCodes key, bool ctrl, bool alt, bool shift, int action);
    std::string buildBindsBlob();
    std::vector<int> chordsForAction(int action);
    void clearMovementOverride();
    void dataLoaded(DS_Dictionary* dict);
    void encodeDataTo(DS_Dictionary* dict);
    int exportBindingsToFile();
    void firstSetup();
    std::vector<int> globalChordsForAction(int action);
    int groupForAction(int action);
    bool init();
    void loadDefaults();
    void parseBindsBlob(std::string const& blob, bool apply, int* outApplied, int* outUnknown);
    int parseImportFile(std::string& outBlob, int& outApplied, int& outUnknown, int& outExtra);
    void unbindChord(int chord, int action);

    std::unordered_map<int, std::set<int>> m_chordToActions;
    std::unordered_map<int, std::set<int>> m_actionToChords;
    std::map<int, std::set<int>> m_savedMovementBinds;
    std::map<int, std::set<int>> m_displacedBinds;
    bool m_movementOverrideActive;

};
