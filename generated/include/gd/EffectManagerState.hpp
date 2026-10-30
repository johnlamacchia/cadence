#pragma once

class CAState;
class CollisionTriggerAction;
class CountTriggerAction;
class GroupCommandObject2;
class OpacityEffectAction;
class PulseEffectAction;
class SpawnTriggerAction;
class TimerItem;
class TimerTriggerAction;
class ToggleTriggerAction;
class TouchToggleAction;

class EffectManagerState {
public:
    EffectManagerState();

    std::vector<CAState> m_colorActions;
    std::vector<PulseEffectAction> m_pulseActions;
    std::unordered_map<int,std::vector<PulseEffectAction>> m_unorderedMapInt_vectorPulseEffectAction;
    std::unordered_map<int,std::vector<CountTriggerAction>> m_unorderedMapInt_vectorCountTriggerAction;
    std::unordered_set<int> m_unorderedSet_int1;
    std::map<int,int> m_mapInt_Int;
    std::unordered_map<int,OpacityEffectAction> m_unorderedMapInt_OpacityEffectAction;
    std::vector<TouchToggleAction> m_vectorTouchToggleAction;
    std::vector<CollisionTriggerAction> m_vectorCollisionTriggerAction;
    std::vector<ToggleTriggerAction> m_vectorToggleTriggerAction;
    std::vector<SpawnTriggerAction> m_vectorSpawnTriggerAction;
    std::unordered_map<int,int> m_itemCountMap;
    std::unordered_map<int,bool> m_unorderedMapInt_bool;
    std::vector<GroupCommandObject2> m_vectorGroupCommandObject2;
    std::unordered_map<int,std::pair<double,double>> m_unorderedMapInt_pair_double_double;
    std::unordered_set<int> m_unorderedSet_int2;
    std::unordered_map<int,TimerItem> m_timerItemMap;
    std::unordered_map<int,std::vector<TimerTriggerAction>> m_unorderedMapInt_vectorTimerTriggerAction;
    int m_platformerProgress;

};
