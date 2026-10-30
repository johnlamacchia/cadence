#pragma once

class GJOnlineRewardDelegate {
public:
    virtual void onlineRewardStatusFinished(std::string key);
    virtual void onlineRewardStatusFailed();

};
