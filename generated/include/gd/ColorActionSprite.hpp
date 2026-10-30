#pragma once

class ColorAction;
class cocos2d;

class ColorActionSprite : public cocos2d::CCNode {
public:
    virtual ~ColorActionSprite();
    virtual bool init();

    static ColorActionSprite* create();

    ColorActionSprite();

    float m_opacity;
    cocos2d::ccColor3B m_color;
    cocos2d::ccColor3B m_copyColor;
    int m_colorID;
    cocos2d::ccColor3B m_pulseColor;
    ColorAction* m_colorAction;

};
