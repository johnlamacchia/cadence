#pragma once

class GJBaseGameLayer;
class cocos2d;

class TimeSpeedControl : public cocos2d::CCNode {
public:
    virtual ~TimeSpeedControl();

    static TimeSpeedControl* create(GJBaseGameLayer*);

    bool init(GJBaseGameLayer*);
    void onAudioTimeSpeedToggle(cocos2d::CCObject* sender);
    void /* TodoReturn */ timeSliderChanged(cocos2d::CCObject*);

};
