#pragma once

class SongInfoObject;
class cocos2d;

class ArtistCell : public TableViewCell {
public:
    virtual ~ArtistCell();
    virtual bool init();
    virtual void draw();

    ArtistCell(char const* identifier, float width, float height);
    void loadFromObject(SongInfoObject* object);
    void onNewgrounds(cocos2d::CCObject* sender);
    void onYouTube(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    SongInfoObject* m_songInfo;

};
