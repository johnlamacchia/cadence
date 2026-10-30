#pragma once

class CCCircleWaveDelegate;
class cocos2d;

class CCCircleWave : public cocos2d::CCNode {
public:
    virtual ~CCCircleWave();
    virtual void setPosition(cocos2d::CCPoint const& position);
    virtual void removeMeAndCleanup();
    virtual void draw();
    virtual void updateTweenAction(float value, char const* key);

    static CCCircleWave* create(float startRadius, float endRadius, float duration, bool fadeIn);
    static CCCircleWave* create(float startRadius, float endRadius, float duration, bool fadeIn, bool easeOut);

    void baseSetup(float radius);
    void followObject(cocos2d::CCNode* newTarget, bool staticPosition);
    bool init(float startRadius, float endRadius, float duration, bool fadeIn, bool easeOut);
    void updatePosition(float dt);

    cocos2d::CCNode* m_target;
    float m_width;
    float m_radius;
    float m_opacity;
    cocos2d::ccColor3B m_color;
    cocos2d::CCPoint m_position;
    CircleMode m_circleMode;
    int m_lineWidth;
    float m_opacityMod;
    bool m_blendAdditive;
    CCCircleWaveDelegate* m_delegate;

};
