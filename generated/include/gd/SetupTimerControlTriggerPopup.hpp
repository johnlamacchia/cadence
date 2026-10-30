#pragma once

class GameObject;
class TimerTriggerGameObject;
class cocos2d;

class SetupTimerControlTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTimerControlTriggerPopup();
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupTimerControlTriggerPopup* create(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(TimerTriggerGameObject* object, cocos2d::CCArray* objects);

};
