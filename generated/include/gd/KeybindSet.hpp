#pragma once

class KeybindSet : public KeybindSource {
public:
    virtual ~KeybindSet();
    virtual std::vector<int> chordsForAction(int action);
    virtual std::vector<int> actionsForChord(int chord);
    virtual void bindChord(int chord, int action);
    virtual void unbindChord(int chord, int action);
    virtual bool isLevelSet();

    KeybindSet();
    void clear();

    std::map<int, std::set<int>> m_actionToChords;
    bool m_dirty;

};
