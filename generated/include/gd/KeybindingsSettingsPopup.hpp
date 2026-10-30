#pragma once

class KeybindSet;
class KeybindSettingsEntry;
class KeybindingsLayer;
class cocos2d;

class KeybindingsSettingsPopup : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~KeybindingsSettingsPopup();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static KeybindingsSettingsPopup* create(KeybindingsLayer* layer, std::vector<KeybindSettingsEntry> const& entries);

    KeybindingsSettingsPopup();
    bool init(KeybindingsLayer* layer, std::vector<KeybindSettingsEntry> const& entries);
    void onClose(cocos2d::CCObject* sender);
    void onEntry(cocos2d::CCObject* sender);
    void onExport(cocos2d::CCObject* sender);
    void onImport(cocos2d::CCObject* sender);
    void onReset(cocos2d::CCObject* sender);

    KeybindingsLayer* m_layer;
    std::vector<KeybindSettingsEntry> m_entries;
    int m_pendingAction;
    std::string m_importBlob;
    int m_importVersion;

};
