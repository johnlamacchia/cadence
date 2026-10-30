#pragma once

class cocos2d;

class CustomSFXRowMenu : public cocos2d::CCMenu {
public:
    virtual ~CustomSFXRowMenu();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static CustomSFXRowMenu* create();

};
