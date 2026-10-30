#pragma once

class LevelListDeleteDelegate {
public:
    virtual void levelListDeleteFinished(int id);
    virtual void levelListDeleteFailed(int id);

};
