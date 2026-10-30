#pragma once

class EditorUI;
class cocos2d;

class GJTransformControlDelegate {
public:
    virtual void transformScaleXChanged(float scaleX);
    virtual void transformScaleYChanged(float scaleY);
    virtual void transformScaleXYChanged(float scaleX, float scaleY);
    virtual void transformRotationXChanged(float rotationX);
    virtual void transformRotationYChanged(float rotationY);
    virtual void transformRotationChanged(float rotation);
    virtual void transformResetRotation();
    virtual void transformRestoreRotation();
    virtual void transformSkewXChanged(float skewX);
    virtual void transformSkewYChanged(float skewY);
    virtual void transformChangeBegin();
    virtual void transformChangeEnded();
    virtual void updateTransformControl();
    virtual void anchorPointMoved(cocos2d::CCPoint anchorPoint);
    virtual cocos2d::CCNode* getTransformNode();
    virtual EditorUI* getUI();

};
