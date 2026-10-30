#pragma once

class ChanceObject {
public:
    ChanceObject();
    ChanceObject(int groupID, int chance);

    int m_groupID;
    int m_oldGroupID;
    int m_chance;
    int m_unk00c;

};
