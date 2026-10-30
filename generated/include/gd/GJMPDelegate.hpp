#pragma once

class GJMPDelegate {
public:
    virtual void joinLobbyFinished(int id);
    virtual void joinLobbyFailed(int id, GJMPErrorCode errorType);
    virtual void didUploadMPComment(int id);
    virtual void updateComments();

};
