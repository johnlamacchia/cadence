#pragma once

class SoundStateContainer;

class FMODSoundState {
public:
    std::string m_filePath;
    float m_speed;
    float m_unkFloat1;
    float m_volume;
    bool m_shouldLoop;
    int m_channelID;
    SoundStateContainer m_soundStateContainer;
    int m_uniqueID;
    int m_sfxGroup;
    float m_pitch;
    bool m_fastFourierTransform;
    bool m_reverb;
    int m_effectID;
    bool m_isMusic;
    int m_musicID;
    bool m_unkBool2;

};
