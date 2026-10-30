#pragma once

class cocos2d;

class GJFriendRequest : public cocos2d::CCNode {
public:
    virtual ~GJFriendRequest();
    virtual bool init();

    static GJFriendRequest* create();
    static GJFriendRequest* create(cocos2d::CCDictionary* dict);

    GJFriendRequest();

    int m_requestID;
    int m_accountID;
    int m_unusedToAccountID;
    std::string m_message;
    std::string m_uploadDate;
    bool m_isRead;

};
