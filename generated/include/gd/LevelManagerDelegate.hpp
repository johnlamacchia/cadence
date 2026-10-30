#pragma once

class cocos2d;

class LevelManagerDelegate {
public:
    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key);
    virtual void loadLevelsFailed(char const* key);
    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type);
    virtual void loadLevelsFailed(char const* key, int type);
    virtual void setupPageInfo(std::string info, char const* key);

};
