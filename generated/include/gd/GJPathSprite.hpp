#pragma once

class GJPathSprite : public CCSpriteCOpacity {
public:
    virtual ~GJPathSprite();

    static GJPathSprite* create(int path);

    void addRankLabel(int path);
    void addShardSprite();
    void changeToLockedArt();
    bool init(int path);
    void updateState();

    int m_pathNumber;

};
