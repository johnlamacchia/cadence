#pragma once

class PlatformDownloadDelegate {
public:
    virtual void downloadFinished(char const* key);
    virtual void downloadFailed(char const* key);

};
