#pragma once

class cocos2d;

class CCContentLayer : public cocos2d::CCLayerColor {
public:
    virtual void setPosition(cocos2d::CCPoint const& position);

    static CCContentLayer* create(cocos2d::ccColor4B const& color, float width, float height);

    CCContentLayer();

};
