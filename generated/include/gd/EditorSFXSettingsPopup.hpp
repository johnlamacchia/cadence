#pragma once

class SFXSettingsPopup;

class EditorSFXSettingsPopup : public SetupTriggerPopup {
public:
    virtual ~EditorSFXSettingsPopup();
    virtual void valueDidChange(int, float);
    virtual float getValue(int);

    static EditorSFXSettingsPopup* create(int);

    bool init(int);

};
