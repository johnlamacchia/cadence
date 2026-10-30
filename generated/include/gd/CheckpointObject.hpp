#pragma once

class EffectManagerState;
class FMOD;
class FMODAudioState;
class GJGameState;
class GJShaderState;
class GameObject;
class PlayerCheckpoint;
class SavedActiveObjectState;
class SavedObjectStateRef;
class SavedSpecialObjectState;
class SequenceTriggerState;
class TimerItem;
class cocos2d;

class CheckpointObject : public cocos2d::CCNode {
public:
    virtual bool init();

    static CheckpointObject* create();

    CheckpointObject();
    ~CheckpointObject();
    GameObject* getObject();
    void setObject(GameObject* object);

    GJGameState m_gameState;
    GJShaderState m_shaderState;
    FMODAudioState m_audioState;
    GameObject* m_physicalCheckpointObject;
    PlayerCheckpoint* m_player1Checkpoint;
    PlayerCheckpoint* m_player2Checkpoint;
    bool m_ground2Invisible;
    bool m_streakBlend;
    int m_uniqueID;
    int m_respawnID;
    std::vector<SavedObjectStateRef> m_vectorSavedObjectStateRef;
    std::vector<SavedActiveObjectState> m_vectorActiveSaveObjectState;
    std::vector<SavedSpecialObjectState> m_vectorSpecialSaveObjectState;
    EffectManagerState m_effectManagerState;
    cocos2d::CCArray* m_gradientTriggerObjectArray;
    std::unordered_map<int,SequenceTriggerState> m_sequenceTriggerStateUnorderedMap;
    int m_commandIndex;
    int m_attempts;
    double m_levelTime;
    double m_bestTime;
    int m_totalClicks;
    double m_timePlayed;
    int m_tickIndex;
    int m_clickIndex;
    int m_jumps;
    std::unordered_map<int, int> m_persistentCounts;
    std::unordered_set<int> m_persistentIDs;
    std::unordered_map<int, TimerItem> m_persistentItems;

};
