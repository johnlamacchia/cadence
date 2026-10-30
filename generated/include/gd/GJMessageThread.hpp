#pragma once

class GJMessageThread : public cocos2d::CCObject {
public:
    virtual ~GJMessageThread();

    static GJMessageThread* create();

    void /* TodoReturn */ createRoleBadge(float);

    int m_threadID;
    int m_accountID;
    std::string m_userName;
    std::string m_groupName;
    int m_unreadCount;
    int m_unk84;
    std::string m_requestMessage;
    std::string m_lastActivity;
    int m_memberCount;
    std::string m_unkd0;
    int m_ownerAccountID;
    bool m_isMuted;
    int m_unkf8;
    std::string m_description;
    std::string m_emoteName;
    bool m_isDM;
    bool m_isRequestPending;
    int m_dmStatus;
    bool m_isLocked;
    bool m_isIgnored;
    bool m_isPinned;
    long long m_unk150;
    int m_modBadge;
    long long m_unk160;
    bool m_isSelected;
    bool m_hasIcon;
    int m_iconID;
    int m_color1;
    int m_color2;
    int m_glowColor;
    int m_iconType;
    int m_special;

};
