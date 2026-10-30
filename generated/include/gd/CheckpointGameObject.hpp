#pragma once

class GJBaseGameLayer;
class GameObject;

class CheckpointGameObject : public EffectGameObject {
public:
    virtual ~CheckpointGameObject();
    virtual bool init();
    virtual void setupCustomSprites(std::string frameName);
    virtual void resetObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void triggerActivated(float xPosition);
    virtual void restoreObject();
    virtual void updateSyncedAnimation(float totalTime, int frameIndex);

    static CheckpointGameObject* create();

    CheckpointGameObject();
    void resetCheckpoint();
    void updateCheckpointSpriteVisibility();

    bool m_checkpointActivated;
    int m_respawnID;
    int m_preSpawnID;

};
