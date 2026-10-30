#pragma once

class GJUserMessage;

class DownloadMessageDelegate {
public:
    virtual void downloadMessageFinished(GJUserMessage* message);
    virtual void downloadMessageFailed(int id);

};
