#pragma once

class cocos2d;

class GJScaleControlDelegate {
public:
    virtual void scaleXChanged(float scaleX, bool lock);
    virtual void scaleYChanged(float scaleY, bool lock);
    virtual void scaleXYChanged(float scaleX, float scaleY, bool lock);
    virtual void scaleChangeBegin();
    virtual void scaleChangeEnded();
    virtual void updateScaleControl();
    virtual void anchorPointMoved(cocos2d::CCPoint newAnchor);

};
