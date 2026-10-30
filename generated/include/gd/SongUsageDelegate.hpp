#pragma once

class SongUsageDelegate {
public:
    virtual void songUsageFinished(int, std::string);
    virtual void songUsageFailed(int);

};
