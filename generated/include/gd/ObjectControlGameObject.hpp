#pragma once

class GJBaseGameLayer;
class GameObject;

class ObjectControlGameObject : public EffectGameObject {
public:
    virtual ~ObjectControlGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static ObjectControlGameObject* create();

    ObjectControlGameObject();

};
