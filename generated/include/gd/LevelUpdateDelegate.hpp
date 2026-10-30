#pragma once

class GJGameLevel;

class LevelUpdateDelegate {
public:
    virtual void levelUpdateFinished(GJGameLevel* level, UpdateResponse response);
    virtual void levelUpdateFailed(int response);

};
