#pragma once

class EditorSoundDeleteDelegate {
public:
    virtual void deleteEditorSoundFinished(int);
    virtual void deleteEditorSoundFailed(int, int);

};
