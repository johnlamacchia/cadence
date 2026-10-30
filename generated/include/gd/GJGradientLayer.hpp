#pragma once

class GradientTriggerObject;
class cocos2d;

class GJGradientLayer : public cocos2d::CCLayerGradient {
public:
    virtual ~GJGradientLayer();
    virtual bool init();

    static GJGradientLayer* create();

    GJGradientLayer();

    int m_baseColor;
    int m_detailColor;
    cocos2d::CCPoint m_gradientPosition;
    GradientTriggerObject* m_triggerObject;
    bool m_noScale;
    int m_blendingMode;
    int m_blendingLayer;
    int m_gradientID;

};
