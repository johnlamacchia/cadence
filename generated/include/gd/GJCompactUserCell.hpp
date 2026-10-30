#pragma once

class GJUserScore;
class cocos2d;

class GJCompactUserCell : public TableViewCell {
public:
    virtual ~GJCompactUserCell();
    virtual bool init();
    virtual void draw();

    GJCompactUserCell(char const*, float, float);
    void /* TodoReturn */ addStat(int, char const*, cocos2d::CCNode*, float&, float, bool);
    void /* TodoReturn */ buildUserRow(GJUserScore*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler, float, bool, bool);
    void /* TodoReturn */ createStatStrip(GJUserScore*, cocos2d::CCNode*, float, float, bool);
    void loadFromScore(GJUserScore*);
    void onSendMessage(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int);

    GJUserScore* m_userScore;

};
