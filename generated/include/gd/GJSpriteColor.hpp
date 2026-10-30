#pragma once

class cocos2d;

class GJSpriteColor {
public:
    GJSpriteColor();
    int getColorMode();

    int m_colorID;
    int m_defaultColorID;
    float m_opacity;
    float m_baseOpacity;
    cocos2d::ccHSVValue m_hsv;
    bool m_usesHSV;
    bool m_usesCustomBlend;
    cocos2d::ccColor3B m_customColor;

};
