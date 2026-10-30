#pragma once

class cocos2d;

class AudioEffectsLayer : public cocos2d::CCLayerColor {
public:
    virtual void draw();
    virtual void updateTweenAction(float value, char const* key);

    static AudioEffectsLayer* create(std::string audioString);

    AudioEffectsLayer();
    ~AudioEffectsLayer();
    void audioStep(float dt);
    cocos2d::CCSprite* getBGSquare();
    void goingDown();
    bool init(std::string audioString);
    void resetAudioVars();
    void triggerEffect(float pulse);

    cocos2d::CCSpriteBatchNode* m_batchNode;
    cocos2d::CCArray* m_masterTicks;
    cocos2d::CCArray* m_tickArray;
    float m_timeElapsed;
    float m_audioPulseMod;
    bool m_goingDown;
    float m_audioScale;
    bool m_audioRainUnused;

};
