#pragma once

class cocos2d;

class LeaderboardManagerDelegate {
public:
    virtual void updateUserScoreFinished();
    virtual void updateUserScoreFailed();
    virtual void loadLeaderboardFinished(cocos2d::CCArray* scores, char const* key);
    virtual void loadLeaderboardFailed(char const* key);

};
