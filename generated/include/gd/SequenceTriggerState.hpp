#pragma once

class SequenceTriggerState {
public:
    std::unordered_map<int, float> m_sequenceTimes;
    std::unordered_map<int, int> m_sequenceIndices;

};
