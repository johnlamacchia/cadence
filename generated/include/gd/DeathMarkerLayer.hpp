#pragma once

class cocos2d;

class DeathMarkerLayer : public cocos2d::CCNode {
public:
    virtual ~DeathMarkerLayer();

    static DeathMarkerLayer* create(std::vector<int> const&, cocos2d::ccColor3B, bool, bool, cocos2d::CCRect const&, int, int, float, float, float, int, int, cocos2d::CCNode*, bool, bool);

    bool init(std::vector<int> const&, cocos2d::ccColor3B, bool, bool, cocos2d::CCRect const&, int, int, float, float, float, int, int, cocos2d::CCNode*, bool, bool);
    void /* TodoReturn */ mountSection(unsigned int);
    void /* TodoReturn */ playMarkerSound();
    void /* TodoReturn */ playPendingLabelSound();
    void /* TodoReturn */ queueDeathLabelSound(int);
    void /* TodoReturn */ resolveBadgeCollisions();
    void setLabelsVisible(bool);
    void /* TodoReturn */ tickDeathCountLabel();
    void /* TodoReturn */ timescale();
    void /* TodoReturn */ unmountSection(unsigned int);
    void /* TodoReturn */ updateVisibility(cocos2d::CCRect const&);
    void setFlipped(bool);

};
