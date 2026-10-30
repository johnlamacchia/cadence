#pragma once

class GameObject;
class TimerTriggerGameObject;
class cocos2d;

class SetupTimerEventTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTimerEventTriggerPopup();

    static SetupTimerEventTriggerPopup* create(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

};
