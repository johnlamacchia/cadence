#pragma once

class FMOD;
class FMODQueuedMusic;
class FMODSound;
class FMODSoundState;
class FMODSoundTween;

class FMODAudioState {
public:
    FMODAudioState();

    float m_interval;
    float m_elapsed;
    std::map<std::pair<int,int>,FMODSoundTween> m_tweensForEffectChannels;
    std::map<std::pair<int,int>,FMODSoundTween> m_tweensForEffectGroups;
    std::map<std::pair<int,int>,FMODSoundTween> m_tweensForMusicChannels;
    std::unordered_map<int,float> m_volumeForEffectChannels;
    std::unordered_map<int,float> m_volumeModForEffectChannels;
    std::unordered_map<int,float> m_pitchForEffectChannels;
    std::unordered_map<int,float> m_volumeForEffectGroups;
    std::unordered_map<int,float> m_volumeModForEffectGroups;
    std::unordered_map<int,float> m_pitchForEffectGroups;
    std::unordered_map<int,float> m_volumeForMusicChannels;
    std::unordered_map<int,float> m_volumeModForMusicChannels;
    std::unordered_map<int,float> m_pitchForMusicChannels;
    std::unordered_map<int,float> m_intervalForEffects;
    std::unordered_map<int,FMODQueuedMusic> m_queuedMusicForChannels1;
    std::unordered_map<int,FMODQueuedMusic> m_queuedMusicForChannels2;
    std::unordered_map<int,FMODSoundState> m_soundStateForChannels;
    uint64_t m_unkUint64_1;

};
