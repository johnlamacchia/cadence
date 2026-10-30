#pragma once

class UploadMessageDelegate {
public:
    virtual void uploadMessageFinished(int accountID);
    virtual void uploadMessageFailed(int accountID);

};
