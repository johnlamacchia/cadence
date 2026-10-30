#pragma once

class CCScrollLayerExt;

class CCScrollLayerExtDelegate {
public:
    virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt* layer);
    virtual void scrollViewDidEndDecelerating(CCScrollLayerExt* layer);
    virtual void scrollViewTouchMoving(CCScrollLayerExt* layer);
    virtual void scrollViewDidEndMoving(CCScrollLayerExt* layer);
    virtual void scrollViewTouchBegin(CCScrollLayerExt* layer);
    virtual void scrollViewTouchEnd(CCScrollLayerExt* layer);

};
