#pragma once

class cocos2d;

class GJSelectionStrip : public cocos2d::CCNode {
public:
    virtual ~GJSelectionStrip();

    static GJSelectionStrip* create(cocos2d::CCMenu*, cocos2d::CCNode*, cocos2d::CCPoint, float, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);

    void /* TodoReturn */ addAction(char const*, float, cocos2d::SEL_MenuHandler, float);
    void /* TodoReturn */ addButton(char const*, float, cocos2d::SEL_MenuHandler, float);
    void /* TodoReturn */ addCountLabel(float, int);
    bool init(cocos2d::CCMenu*, cocos2d::CCNode*, cocos2d::CCPoint, float, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
    void setCount(int);
    void setStripVisible(bool);

};
