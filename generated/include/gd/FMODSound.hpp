#pragma once

class FMOD;

class FMODSound {
public:
    FMOD::Sound* m_sound;
    std::string m_filePath;
    int m_length;
    int m_playCount;
    int m_playIndex;
    bool m_preloaded;

};
