#pragma once

class cocos2d;

class GJTransformState {
public:
    GJTransformState();

    float m_scaleX;
    float m_scaleY;
    float m_angleX;
    float m_angleY;
    float m_skewX;
    float m_skewY;
    float m_transformRotation;
    bool m_transformReset;
    float m_transformRotationX;
    float m_transformRotationY;
    cocos2d::CCPoint m_transformPosition;
    cocos2d::CCPoint m_transformSkewX;
    cocos2d::CCPoint m_transformSkewY;
    float m_transformScaleX;
    float m_transformScaleY;

};
