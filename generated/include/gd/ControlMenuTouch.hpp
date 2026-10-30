#pragma once

class cocos2d;

class ControlMenuTouch {
public:
    void addMenu(cocos2d::CCMenu* menu);
    bool began(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void cancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void ended(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    void moved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    bool tracking();

    std::vector<cocos2d::CCMenu*> m_menus;
    cocos2d::CCMenu* m_activeMenu;

};
