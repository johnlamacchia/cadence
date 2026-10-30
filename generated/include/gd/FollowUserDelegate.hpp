#pragma once

class FollowUserDelegate {
public:
    virtual void followUserFinished(int, bool);
    virtual void followUserFailed(int, bool);

};
