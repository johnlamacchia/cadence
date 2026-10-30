#pragma once

class KeybindSource {
public:
    virtual ~KeybindSource();
    virtual std::vector<int> chordsForAction(int action);
    virtual std::vector<int> actionsForChord(int chord);
    virtual void bindChord(int chord, int action);
    virtual void unbindChord(int chord, int action);
    virtual bool isLevelSet();

    KeybindSource();

};
