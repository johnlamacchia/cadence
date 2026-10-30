#pragma once

class SFXInfoObject;

class CustomSFXCell : public TableViewCell, public CustomSFXDelegate {
public:
    virtual ~CustomSFXCell();
    virtual bool init();
    virtual void draw();
    virtual void sfxObjectSelected(SFXInfoObject* object);
    virtual int getActiveSFXID();
    virtual int getActiveCustomSFXID();
    virtual void sfxAddToFolder(SFXInfoObject*);
    virtual void sfxEditCustomObject(SFXInfoObject*);
    virtual void sfxToggleCustomSelect(SFXInfoObject*);
    virtual bool isCustomEntrySelected(int);
    virtual bool inCustomSFXTree();
    virtual bool showsSFXInfoButton();

    CustomSFXCell(char const* identifier, float width, float height);
    void loadFromObject(SFXInfoObject* object);
    bool shouldReload();
    void updateBGColor(int index);

    SFXInfoObject* m_sfxObject;
    bool m_selected;
    bool m_compactMode;

};
