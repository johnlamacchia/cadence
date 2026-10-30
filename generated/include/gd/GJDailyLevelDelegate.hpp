#pragma once

class GJDailyLevelDelegate {
public:
    virtual void dailyStatusFinished(GJTimedLevelType type);
    virtual void dailyStatusFailed(GJTimedLevelType type, GJErrorCode errorType);

};
