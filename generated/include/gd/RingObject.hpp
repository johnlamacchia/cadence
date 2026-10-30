#pragma once

class GJBaseGameLayer;

class RingObject : public EffectGameObject {
public:
    virtual ~RingObject();
    virtual void setScale(float scale);
    virtual void setRotation(float rotation);
    virtual void resetObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void setRScale(float scale);
    virtual void triggerActivated(float xPosition);
    virtual bool shouldDrawEditorHitbox();
    virtual void powerOnObject(int state);

    static RingObject* create(char const* frame);

    RingObject();
    bool init(char const* frame);
    void spawnCircle();

    bool m_claimTouch;
    bool m_isSpawnOnly;

};
