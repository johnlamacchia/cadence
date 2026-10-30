#pragma once

class ColorAction;
class ColorActionSprite;
class cocos2d;

class ColorAction2 {
public:
    ColorAction2(cocos2d::ccColor3B fromColor, cocos2d::ccColor3B toColor, float duration, double unused, bool blending, int playerColor, float fromOpacity, float toOpacity);
    void resetAction();
    void step(float dt);
    void updateCustomColor(cocos2d::ccColor3B color1, cocos2d::ccColor3B color2);

    bool m_stepFinished;
    bool m_paused;
    cocos2d::ccColor3B m_color;
    float m_currentOpacity;
    float m_deltaTime;
    cocos2d::ccColor3B m_fromColor;
    cocos2d::ccColor3B m_toColor;
    float m_duration;
    bool m_blending;
    int m_playerColor;
    int m_colorID;
    float m_fromOpacity;
    float m_toOpacity;
    cocos2d::ccHSVValue m_copyHSV;
    int m_copyID;
    bool m_copyColorCalculated;
    bool m_copyOpacity;
    bool m_copyColorLoop;
    int m_uniqueID;
    bool m_legacyHSV;
    ColorActionSprite* m_colorSprite;
    float m_actionDelay;
    float m_stepInterval;

};
