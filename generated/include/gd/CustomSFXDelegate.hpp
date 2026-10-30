#pragma once

class SFXInfoObject;

class CustomSFXDelegate {
public:
    virtual void sfxObjectSelected(SFXInfoObject* object);
    virtual int getActiveSFXID();
    virtual int getActiveCustomSFXID();
    virtual bool overridePlaySFX(SFXInfoObject* object);
    virtual void sfxAddToFolder(SFXInfoObject*);
    virtual void sfxEditCustomObject(SFXInfoObject*);
    virtual void sfxToggleCustomSelect(SFXInfoObject*);
    virtual bool isCustomEntrySelected(int);
    virtual bool inCustomSFXTree();
    virtual bool showsSFXInfoButton();

};
