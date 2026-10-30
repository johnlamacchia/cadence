#pragma once

class SongInfoObject;

class CustomMusicCell : public CustomSongCell {
public:
    virtual ~CustomMusicCell();

    CustomMusicCell(char const* identifier, float width, float height);
    void loadFromObject(SongInfoObject* object);
    void updateBGColor(int index);

};
