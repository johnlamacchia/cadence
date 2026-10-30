#pragma once

class cocos2d;

class EditorSoundListDelegate {
public:
    virtual void loadEditorSoundsFinished(cocos2d::CCArray*, char const*);
    virtual void loadEditorSoundsFailed(char const*);

};
