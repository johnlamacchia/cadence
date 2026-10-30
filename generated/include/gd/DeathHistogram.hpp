#pragma once

class cocos2d;

class DeathHistogram : public cocos2d::CCNode {
public:
    virtual ~DeathHistogram();

    static DeathHistogram* create(cocos2d::CCSize);

    void /* TodoReturn */ handleTouchBegan(cocos2d::CCTouch*);
    void /* TodoReturn */ handleTouchEnded(cocos2d::CCTouch*);
    void /* TodoReturn */ handleTouchMoved(cocos2d::CCTouch*);
    bool init(cocos2d::CCSize);
    void /* TodoReturn */ persistScroll(std::string const&);
    void /* TodoReturn */ rebuildAxis(char const*, int, float, float, char const*);
    void /* TodoReturn */ restoreScroll(std::string const&);
    void setAttemptLine(std::vector<int> const&, std::vector<int> const&, cocos2d::ccColor3B, int, cocos2d::ccColor3B, bool);
    void setData(std::vector<int> const&, int, bool, bool, bool);
    void setProgression(std::vector<int> const&, std::vector<int> const&, std::vector<int> const&);
    void setSurvival(std::vector<int> const&, bool, bool, int);

};
