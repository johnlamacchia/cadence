#pragma once

class cocos2d;

class ListCell : public TableViewCell {
public:
    virtual ~ListCell();
    virtual bool init();
    virtual void draw();

    ListCell(char const* identifier, float width, float height);
    void loadFromObject(cocos2d::CCObject* object, int id, int page, int index);
    void updateBGColor(int index);

    cocos2d::ccColor4B m_bgColor;

};
