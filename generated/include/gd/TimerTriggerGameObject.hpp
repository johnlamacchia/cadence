#pragma once

class GJBaseGameLayer;
class GameObject;

class TimerTriggerGameObject : public EffectGameObject {
public:
    virtual ~TimerTriggerGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static TimerTriggerGameObject* create(char const* frame);

    TimerTriggerGameObject();
    bool init(char const* frame);

    double m_startTime;
    double m_targetTime;
    bool m_stopTimeEnabled;
    bool m_dontOverride;
    bool m_ignoreTimeWarp;
    float m_timeMod;
    bool m_startPaused;
    bool m_multiActivate;
    int m_controlType;

};
