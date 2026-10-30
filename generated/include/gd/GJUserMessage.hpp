#pragma once

class cocos2d;

class GJUserMessage : public cocos2d::CCNode {
public:
    virtual ~GJUserMessage();
    virtual bool init();

    static GJUserMessage* create();
    static GJUserMessage* create(cocos2d::CCDictionary* dict);

    GJUserMessage();

    int m_messageID;
    int m_accountID;
    int m_userID;
    std::string m_title;
    std::string m_content;
    std::string m_username;
    std::string m_uploadDate;
    bool m_read;
    bool m_outgoing;
    bool m_toggled;

};
