#pragma once

class LevelSettingsObject;
class OptionsLayer;
class cocos2d;

class LevelOptionsLayer : public GJOptionsLayer {
public:
    virtual ~LevelOptionsLayer();
    virtual void valueDidChange(int tag, float value);
    virtual float getValue(int tag);
    virtual void setupOptions();
    virtual void didToggle(int tag);

    static LevelOptionsLayer* create(LevelSettingsObject* object);

    LevelOptionsLayer();
    bool init(LevelSettingsObject* object);
    void onSettings(cocos2d::CCObject* sender);

    LevelSettingsObject* m_settingsObject;

};
