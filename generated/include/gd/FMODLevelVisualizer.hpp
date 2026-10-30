#pragma once

class FMOD;

class FMODLevelVisualizer : public cocos2d::CCNode {
public:
    virtual ~FMODLevelVisualizer();
    virtual bool init();

    static FMODLevelVisualizer* create();

    void updateVisualizer(float volume, float peak, float time);

    float m_width;

};
