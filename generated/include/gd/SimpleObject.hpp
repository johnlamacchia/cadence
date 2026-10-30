#pragma once

class cocos2d;

class SimpleObject : public cocos2d::CCObject {
public:
    static SimpleObject* create();

    SimpleObject();
    bool init();

    cocos2d::ccColor3B m_color;

};
