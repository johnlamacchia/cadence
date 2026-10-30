#pragma once

class UploadActionDelegate {
public:
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);

};
