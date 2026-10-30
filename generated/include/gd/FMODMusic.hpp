#pragma once

class FMOD;

class FMODMusic {
public:
    int m_channelID;
    FMOD::Sound* m_sound;
    std::string m_filePath;
    bool m_ogg;
    bool m_dontReset;

};
