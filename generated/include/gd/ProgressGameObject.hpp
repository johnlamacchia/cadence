#pragma once

class GJBaseGameLayer;
class GameObject;

class ProgressGameObject : public EffectGameObject {
public:
    virtual ~ProgressGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>&, std::vector<void*>&);
    virtual std::string getSaveString(GJBaseGameLayer*);

    static ProgressGameObject* create();

};
