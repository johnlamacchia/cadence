#pragma once

class SFXFolderObject;

class SFXSearchResult : public MusicSearchResult {
public:
    virtual ~SFXSearchResult();
    virtual void updateObjects(AudioSortType type);

    static SFXSearchResult* create(int folderID);

    SFXFolderObject* getActiveFolder();
    int getSelectedPage(int id, int count);
    bool init(int folderID);
    void setActiveFolder(SFXFolderObject* folder);

    int m_folderID;
    SFXFolderObject* m_folderObject;

};
