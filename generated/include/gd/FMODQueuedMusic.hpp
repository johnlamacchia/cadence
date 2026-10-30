#pragma once

class FMOD;

class FMODQueuedMusic {
public:
    std::string m_filePath;
    float m_pitch;
    float m_unkFloat2;
    float m_volume;
    int m_start;
    int m_end;
    int m_fadeIn;
    int m_fadeOut;
    bool m_loop;
    int m_musicID;
    bool m_stopMusic;
    int m_channelID;
    bool m_noPrepare;
    int m_queuedStatus;
    FMOD::Sound* m_sound;
    bool m_dontReset;

};
