#pragma once

class GJAccountBackupDelegate {
public:
    virtual void backupAccountFinished();
    virtual void backupAccountFailed(BackupAccountError errorType, int response);

};
