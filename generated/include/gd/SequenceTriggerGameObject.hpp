#pragma once

class GJBaseGameLayer;
class GameObject;
class SequenceTriggerState;

class SequenceTriggerGameObject : public ChanceTriggerGameObject {
public:
    virtual ~SequenceTriggerGameObject();
    virtual bool init();
    virtual void resetObject();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SequenceTriggerGameObject* create();

    SequenceTriggerGameObject();
    void addCount(int index, int count);
    void addTarget(int groupID, int count);
    void deleteTarget(int index);
    bool reorderTarget(int index, bool left);
    void updateSequenceTotalCount();

    SequenceTriggerState m_sequenceState;
    float m_minInt;
    int m_sequenceMode;
    int m_resetMode;
    float m_reset;
    int m_sequenceTotalCount;
    bool m_uniqueRemap;

};
