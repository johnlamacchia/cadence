#pragma once

class SongObject;
class cocos2d;

class SongCell : public TableViewCell {
public:
    virtual ~SongCell();
    virtual bool init();
    virtual void draw();

    SongCell(char const* identifier, float width, float height);
    void loadFromObject(SongObject* object);
    void onClick(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    SongObject* m_songObject;

};
