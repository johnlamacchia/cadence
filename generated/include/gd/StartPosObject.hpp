#pragma once

class GJBaseGameLayer;
class LevelSettingsObject;

class StartPosObject : public EffectGameObject {
public:
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static StartPosObject* create();

    StartPosObject();
    ~StartPosObject();
    void /* TodoReturn */ getDisplayName();
    void loadSettingsFromString(std::string objectString);
    void setSettings(LevelSettingsObject* settings);

    LevelSettingsObject* m_startSettings;

};
