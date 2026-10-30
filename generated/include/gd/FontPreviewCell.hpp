#pragma once

class cocos2d;

class FontPreviewCell : public TableViewCell {
public:
    virtual ~FontPreviewCell();
    virtual bool init();
    virtual void draw();

    FontPreviewCell(char const*, float, float);
    void /* TodoReturn */ loadFromIndex(int, int);
    void onSelectFont(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateBGColor(int);

};
