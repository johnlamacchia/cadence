#pragma once

class GJGameLevel;

class GJDifficultySprite : public cocos2d::CCSprite {
public:
    virtual ~GJDifficultySprite();

    static GJDifficultySprite* create(int difficulty, GJDifficultyName name);
    static std::string getDifficultyFrame(int difficulty, GJDifficultyName name);

    bool init(int difficulty, GJDifficultyName name);
    void updateDifficultyFrame(int difficulty, GJDifficultyName name);
    void updateFeatureState(GJFeatureState state);
    void updateFeatureStateFromLevel(GJGameLevel* level);

    GJFeatureState m_featureState;

};
