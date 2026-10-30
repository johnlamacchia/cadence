#pragma once

class GJBaseGameLayer;
class GameObject;

class ArtTriggerGameObject : public EffectGameObject {
public:
    virtual ~ArtTriggerGameObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static ArtTriggerGameObject* create(char const* frame);

    ArtTriggerGameObject();
    bool init(char const* frame);

    int m_artIndex;

};
