#pragma once

class GJGameLevel;

class LevelUploadDelegate {
public:
    virtual void levelUploadFinished(GJGameLevel* level);
    virtual void levelUploadFailed(GJGameLevel* level);

};
