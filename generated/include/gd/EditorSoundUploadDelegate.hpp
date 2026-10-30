#pragma once

class EditorSoundUploadDelegate {
public:
    virtual void uploadEditorSoundFinished(int);
    virtual void uploadEditorSoundFailed(int);

};
