#pragma once

class SongInfoObject;

class MusicDownloadDelegate {
public:
    virtual void loadSongInfoFinished(SongInfoObject* object);
    virtual void loadSongInfoFailed(int id, GJSongError errorType);
    virtual void downloadSongStarted(int id);
    virtual void downloadSongFinished(int id);
    virtual void downloadSongFailed(int id, GJSongError errorType);
    virtual void songStateChanged();
    virtual void downloadSFXFinished(int id);
    virtual void downloadSFXFailed(int id, GJSongError errorType);
    virtual void musicActionFinished(GJMusicAction action);
    virtual void musicActionFailed(GJMusicAction action);
    virtual void /* TodoReturn */ newsImageDownloaded(std::string);
    virtual void /* TodoReturn */ newsImageFailed(std::string);

};
