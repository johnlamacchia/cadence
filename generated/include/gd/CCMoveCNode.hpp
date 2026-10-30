#pragma once

class GroupCommandObject2;

class CCMoveCNode : public cocos2d::CCObject {
public:
    virtual ~CCMoveCNode();

    static CCMoveCNode* create();

    CCMoveCNode();
    bool init();
    void reset();

    double m_unk038;
    double m_unk040;
    double m_unk048;
    void* m_unk050;
    double m_unk058;
    double m_unk060;
    double m_unk068;
    bool m_unk070;
    bool m_unk071;
    int m_unk074;
    bool m_unk078;
    int m_unk07c;
    double m_unk080;
    double m_unk088;
    double m_unk090;
    double m_unk098;
    double m_unk0a0;
    double m_unk0a8;
    double m_unk0b0;
    int m_unk0b8;
    double m_unk0c0;
    GroupCommandObject2* m_groupObject;
    bool m_unk0d0;
    bool m_unk0d1;
    std::vector<GroupCommandObject2*> m_groupObjects;

};
