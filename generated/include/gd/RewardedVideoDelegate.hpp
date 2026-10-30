#pragma once

class RewardedVideoDelegate {
public:
    virtual void rewardedVideoFinished();
    virtual bool shouldOffsetRewardCurrency();

};
