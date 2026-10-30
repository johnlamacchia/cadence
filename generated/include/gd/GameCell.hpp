#pragma once

class cocos2d;

class GameCell : public TableViewCell {
public:
    virtual ~GameCell();
    virtual bool init();
    virtual void draw();

    GameCell(char const* identifier, float width, float height);
    void loadFromString(std::string str);
    void onTouch(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    void* m_unk230;
    std::string m_gameLink;

};
