#pragma once

class GJBaseGameLayer;
class GameObject;

class AudioLineGuideGameObject : public EffectGameObject {
public:
    virtual ~AudioLineGuideGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static AudioLineGuideGameObject* create();

    AudioLineGuideGameObject();

    int m_beatsPerMinute;
    int m_beatsPerBar;
    Speed m_speed;
    bool m_disabled;

};
