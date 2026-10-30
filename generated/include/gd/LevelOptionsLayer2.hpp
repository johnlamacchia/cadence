#pragma once

class LevelSettingsObject;
class OptionsLayer;

class LevelOptionsLayer2 : public LevelOptionsLayer {
public:
    virtual ~LevelOptionsLayer2();
    virtual void setupOptions();

    static LevelOptionsLayer2* create(LevelSettingsObject* object);

    LevelOptionsLayer2();
    bool init(LevelSettingsObject* object);

};
