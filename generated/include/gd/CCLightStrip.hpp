#pragma once

class cocos2d;

class CCLightStrip : public cocos2d::CCNode {
public:
    virtual ~CCLightStrip();
    virtual void draw();
    virtual void updateTweenAction(float value, char const* keyword);

    static CCLightStrip* create(float width, float toWidth, float toHeight, float duration, float delay);

    CCLightStrip();
    bool init(float width, float toWidth, float toHeight, float duration, float delay);

    float m_objectWidth;
    float m_toWidth;
    float m_toHeight;
    float m_duration;
    cocos2d::ccColor4B m_color;
    float m_opacity;
    float m_width;
    float m_height;

};
