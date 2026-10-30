#pragma once

class cocos2d;

class DungeonBarsSprite : public cocos2d::CCNode {
public:
    virtual ~DungeonBarsSprite();
    virtual bool init();
    virtual void visit();

    static DungeonBarsSprite* create();

    DungeonBarsSprite();
    void animateOutBars();

    cocos2d::CCSprite* m_barsSprite;

};
