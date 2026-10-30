#pragma once

class GameObject;
class TimerTriggerGameObject;
class cocos2d;

class SetupTimerTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTimerTriggerPopup();

    static SetupTimerTriggerPopup* create(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

};
