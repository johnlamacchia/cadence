#pragma once

class RecordCheckpoint {
public:
    int m_index;
    int m_step;
    uint64_t m_seed;
    int m_attempts;
    int m_ticks;
    uint64_t m_time;
    int m_clicks;
    int m_points;
    std::string m_persistentState;

};
