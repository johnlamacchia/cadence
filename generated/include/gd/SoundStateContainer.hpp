#pragma once

class SoundStateContainer {
public:
    int m_fadePointCount;
    std::array<float, 4> m_fadePointVolumes;
    std::array<uint64_t, 4> m_fadePointOffsets;
    uint64_t m_currentOffset;
    int m_loopStartMs;
    int m_loopEndMs;
    int m_currentMs;
    uint64_t m_playStartOffset;
    uint64_t m_playEndOffset;
    int m_usePlayOffsets;

};
