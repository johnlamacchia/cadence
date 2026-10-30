#pragma once

class cocos2d;

class DynamicScrollDelegate {
public:
    virtual void updatePageWithObject(cocos2d::CCObject* layer, cocos2d::CCObject* object);

};
