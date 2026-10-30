#pragma once

class GJUserScore;
class cocos2d;

class GJComment : public cocos2d::CCNode {
public:
    virtual bool init();

    static GJComment* create();
    static GJComment* create(cocos2d::CCDictionary* dict);

    GJComment();
    ~GJComment();

    std::string m_commentString;
    std::string m_userName;
    int m_commentID;
    int m_userID;
    int m_likeCount;
    int m_levelID;
    bool m_isSpam;
    int m_accountID;
    std::string m_uploadDate;
    bool m_commentDeleted;
    int m_percentage;
    int m_modBadge;
    bool m_isFriend;
    cocos2d::ccColor3B m_color;
    bool m_hasLevelID;
    bool m_unkMultiplayerBool;
    bool m_canDelete;
    int m_commentType;
    bool m_isSelected;
    int m_unk1d4;
    GJUserScore* m_userScore;

};
