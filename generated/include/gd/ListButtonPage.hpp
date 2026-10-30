#pragma once

class ButtonPage;
class cocos2d;

class ListButtonPage : public cocos2d::CCLayer {
public:
    virtual ~ListButtonPage();

    static ListButtonPage* create(cocos2d::CCArray* items, cocos2d::CCPoint position, int columns, int rows, float columnOffset, float rowOffset, float offset);

    bool init(cocos2d::CCArray* items, cocos2d::CCPoint position, int columns, int rows, float columnOffset, float rowOffset, float offset);

};
