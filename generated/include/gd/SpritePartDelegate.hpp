#pragma once

class cocos2d;

class SpritePartDelegate {
public:
    virtual void displayFrameChanged(cocos2d::CCObject* sprite, std::string frameName);

};
