#pragma once

class LevelDeleteDelegate {
public:
    virtual void levelDeleteFinished(int id);
    virtual void levelDeleteFailed(int id);

};
