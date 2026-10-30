#pragma once

class DS_Dictionary;
class EditorSFXBinding;
class EditorSFXImportOptions;
class GJEditorSoundConfig;
class GameObject;
class SFXTriggerGameObject;

class EditorSFXManager {
public:
    static EditorSFXManager* sharedState();

    void /* TodoReturn */ actionToString(int);
    void /* TodoReturn */ addFavoriteConfig(GJEditorSoundConfig*);
    void /* TodoReturn */ addSound(int, SFXTriggerGameObject*);
    void /* TodoReturn */ allActions();
    void /* TodoReturn */ applyConfigDirect(GJEditorSoundConfig*, bool, std::string*);
    void /* TodoReturn */ applyFavoriteConfig(int, bool, std::string*);
    void /* TodoReturn */ applyMyConfig(int, bool, std::string*);
    void /* TodoReturn */ attachUploadedSoundID(int, int);
    void /* TodoReturn */ clearAction(int);
    void /* TodoReturn */ clearActionMinInterval(int);
    void /* TodoReturn */ clearAll();
    void /* TodoReturn */ collectAllSFXIDs();
    void /* TodoReturn */ copyFavoriteToMyConfig(GJEditorSoundConfig*);
    void /* TodoReturn */ createMyConfigFromLive(std::string const&);
    void /* TodoReturn */ createNewSound(int);
    void /* TodoReturn */ dataLoaded(DS_Dictionary*);
    void /* TodoReturn */ decodeSound(std::string const&);
    void /* TodoReturn */ deserializeAll(std::string const&, bool);
    void /* TodoReturn */ downloadAllMissing();
    void /* TodoReturn */ encodeDataTo(DS_Dictionary*);
    void /* TodoReturn */ encodeSound(SFXTriggerGameObject*);
    void /* TodoReturn */ ensureLoaded();
    void /* TodoReturn */ findBinding(int);
    void /* TodoReturn */ firstSetup();
    void /* TodoReturn */ getActionMinInterval(int);
    void /* TodoReturn */ getActionMinIntervalOverride(int);
    void /* TodoReturn */ getCustomSFXRawPath(int, int);
    void /* TodoReturn */ getFavoriteConfig(int);
    void /* TodoReturn */ getFavoriteConfigs();
    void /* TodoReturn */ getMyConfig(int);
    void /* TodoReturn */ getMyConfigForSoundID(int);
    void /* TodoReturn */ getMyConfigs();
    void /* TodoReturn */ getOrCreateBinding(int);
    void /* TodoReturn */ getPlayMode(int);
    void /* TodoReturn */ hasSoundsForAction(int);
    void /* TodoReturn */ importFromString(std::string const&, EditorSFXImportOptions const&, std::string*);
    bool init();
    void /* TodoReturn */ overwriteMyConfigFromLive(int, std::string*);
    void playAction(int);
    void /* TodoReturn */ playAction(int, int);
    void /* TodoReturn */ releaseBinding(EditorSFXBinding*);
    void /* TodoReturn */ removeFavoriteConfig(int);
    void /* TodoReturn */ removeMyConfig(int);
    void /* TodoReturn */ removeSoundAt(int, int);
    void /* TodoReturn */ serializeAll();
    void /* TodoReturn */ serializeCompressed();
    void setActionMinInterval(int, float);
    void setEditorLoading(bool);
    void setGlobalVolume(float);
    void setPlayMode(int, int);
    void /* TodoReturn */ soundCountForAction(int);
    void /* TodoReturn */ soundsForAction(int);
    void /* TodoReturn */ summarizeBlob(std::string const&);

};
