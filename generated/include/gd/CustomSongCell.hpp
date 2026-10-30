#pragma once

class LevelSettingsObject;
class SongInfoObject;
class cocos2d;

class CustomSongCell : public TableViewCell, public CustomSongDelegate {
public:
    virtual ~CustomSongCell();
    virtual bool init();
    virtual void draw();
    virtual void songIDChanged(int id);
    virtual int getActiveSongID();
    virtual std::string getSongFileName();
    virtual LevelSettingsObject* getLevelSettings();

    CustomSongCell(char const* identifier, float width, float height);
    void loadFromObject(SongInfoObject* object);
    void onDelete(cocos2d::CCObject* sender);
    bool shouldReload();
    void updateBGColor(int index);

    SongInfoObject* m_songInfoObject;
    bool m_selected;

};
