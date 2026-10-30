#pragma once

class GJAccountSyncDelegate {
public:
    virtual void syncAccountFinished();
    virtual void syncAccountFailed(BackupAccountError errorType, int response);

};
