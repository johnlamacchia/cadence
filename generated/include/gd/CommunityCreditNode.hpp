#pragma once

class CommunityCreditNode : public cocos2d::CCNode {
public:
    virtual ~CommunityCreditNode();

    static CommunityCreditNode* create(int unlockType, int iconID, int unknown, std::string author);

    bool init(int unlockType, int iconID, int unknown, std::string author);

    int m_unlockType;
    int m_iconID;
    int m_unknown;
    std::string m_author;

};
