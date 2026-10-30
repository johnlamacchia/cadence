#pragma once

class GJGameLevel;

class LevelDownloadDelegate {
public:
    virtual void levelDownloadFinished(GJGameLevel* level);
    virtual void levelDownloadFailed(int response);

};
