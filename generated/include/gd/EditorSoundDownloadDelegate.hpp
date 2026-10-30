#pragma once

class GJEditorSoundConfig;

class EditorSoundDownloadDelegate {
public:
    virtual void downloadEditorSoundFinished(GJEditorSoundConfig*);
    virtual void downloadEditorSoundFailed(int, int);

};
