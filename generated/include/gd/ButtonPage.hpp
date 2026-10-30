#pragma once

class cocos2d;

class ButtonPage : public cocos2d::CCLayer {
public:
    virtual ~ButtonPage();

    static ButtonPage* create(cocos2d::CCArray* objects, cocos2d::CCPoint position, int rows, float spacing);

    ButtonPage();
    bool init(cocos2d::CCArray* objects, cocos2d::CCPoint position, int rows, float spacing);

};
