#pragma once

class GJRewardDelegate {
public:
    virtual void rewardsStatusFinished(int type);
    virtual void rewardsStatusFailed();

};
