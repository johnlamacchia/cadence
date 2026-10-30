#pragma once

class CCCircleAlert;

class CCAlertCircle : public cocos2d::CCNode {
public:
    virtual bool init();
    virtual void draw();

    static CCAlertCircle* create();

    CCCircleAlert* m_circleAlert;

};
