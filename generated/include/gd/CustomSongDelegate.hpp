#pragma once

class LevelSettingsObject;

class CustomSongDelegate {
public:
    virtual void songIDChanged(int id);
    virtual int getActiveSongID();
    virtual std::string getSongFileName();
    virtual LevelSettingsObject* getLevelSettings();

};
