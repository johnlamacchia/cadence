#pragma once

class EditorBuildStats {
public:
    void fromString(std::string const&);
    void noteAction();
    std::string toString();

    int m_objectsCreated;
    int m_objectsPasted;
    int m_objectsDeleted;
    int m_transformActions;
    int m_editPopupsOpened;
    int m_undoRedo;
    int m_playtestCount;
    float m_playtestTime;
    int m_editorFrames;
    int m_editorInputs;
    int m_editorSessions;
    uint64_t m_firstOpen;
    int m_originalLevelID;
    int m_originalLevelVersion;
    std::array<int, 8> m_actionGaps;
    int m_lastActionTime;

};
