#pragma once

class LevelLeaderboard;

class GJLocalScore : public cocos2d::CCObject {
public:
    virtual ~GJLocalScore();

    static GJLocalScore* create(int points, int type);

    GJLocalScore();
    bool init(int points, int type);

    int m_points;
    LevelLeaderboardType m_type;
    bool m_currentScore;

};
