#pragma once

class GJUserScore;
class cocos2d;

class GJLevelScoreCell : public TableViewCell {
public:
    virtual ~GJLevelScoreCell();
    virtual bool init();
    virtual void draw();

    GJLevelScoreCell(char const* identifier, float width, float height);
    void loadFromScore(GJUserScore* score);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);

    GJUserScore* m_userScore;

};
