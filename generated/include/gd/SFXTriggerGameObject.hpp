#pragma once

class FMOD;
class GJBaseGameLayer;
class GameObject;

class SFXTriggerGameObject : public EffectGameObject {
public:
    virtual ~SFXTriggerGameObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SFXTriggerGameObject* create(char const* frame);

    SFXTriggerGameObject();
    int getSFXRefID();
    int getUniqueSFXID();
    bool init(char const* frame);

    std::string m_soundPath;
    int m_soundID;
    int m_property601;
    float m_pitch;
    int m_speed;
    int m_pitchIndex;
    float m_volume;
    int m_start;
    int m_fadeIn;
    int m_end;
    int m_fadeOut;
    bool m_reverb;
    bool m_fastFourierTransform;
    bool m_loop;
    bool m_stopLoop;
    bool m_dontReset;
    bool m_unique;
    bool m_override;
    int m_sfxUniqueID;
    float m_volumeNear;
    float m_volumeMedium;
    float m_volumeFar;
    int m_minDistNear;
    int m_minDistMedium;
    int m_minDistFar;
    int m_proximityMode;
    bool m_cameraDistance;
    bool m_preload;
    bool m_ignoreVolumeTest;
    float m_minInterval;
    int m_sfxGroup;
    bool m_stop;
    bool m_changeSpeed;
    bool m_changeVolume;
    int m_groupID;
    int m_unk788;
    FMODReverbPreset m_reverbPreset;
    bool m_reverbEnabled;
    float m_soundDuration;
    bool m_applyDisabled;
    int m_speedVariance;
    int m_pitchVariance;
    float m_volumeVariance;
    bool m_pitchSteps;
    std::string m_unk7e8;

};
