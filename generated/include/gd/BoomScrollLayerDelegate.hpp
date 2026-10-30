#pragma once

class BoomScrollLayer;
class cocos2d;

class BoomScrollLayerDelegate {
public:
    virtual void scrollLayerScrollingStarted(BoomScrollLayer* layer);
    virtual void scrollLayerScrolledToPage(BoomScrollLayer* layer, int page);
    virtual void scrollLayerMoved(cocos2d::CCPoint position);
    virtual void scrollLayerWillScrollToPage(BoomScrollLayer* layer, int page);

};
