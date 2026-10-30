#pragma once

class BoomScrollLayer;
class BoomScrollLayerDelegate;
class cocos2d;

class ExtendedLayer : public cocos2d::CCLayer {
public:
    virtual ~ExtendedLayer();
    virtual bool init();
    virtual void setPosition(cocos2d::CCPoint const& position);

    static ExtendedLayer* create();

    ExtendedLayer();

    BoomScrollLayerDelegate* m_delegate;

};
