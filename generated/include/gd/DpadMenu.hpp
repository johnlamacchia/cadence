#pragma once

class cocos2d;

class DpadMenu : public cocos2d::CCMenu {
public:
    virtual ~DpadMenu();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static DpadMenu* create(cocos2d::CCPoint, cocos2d::CCSize, cocos2d::CCSize);

    void /* TodoReturn */ arrowForTouch(cocos2d::CCTouch*);

};
