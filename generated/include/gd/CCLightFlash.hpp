#pragma once

class cocos2d;

class CCLightFlash : public cocos2d::CCNode {
public:
    virtual bool init();

    static CCLightFlash* create();

    CCLightFlash();
    ~CCLightFlash();
    void cleanupFlash();
    void fadeAndRemove();
    void playEffect(cocos2d::CCPoint pos, cocos2d::ccColor3B col, float startWidth, float startWidthVariation, float endWidth, float endWidthVariation, float endHeight, float stripDuration, float stripDurationVariance, float stripInterval, float stripStartDelay, float stripStartDelayVariation, float stripRotation, float stripRotationVariation, float stripOpacity, float stripOpacityVariation, int stripCount, bool circleRotation, bool fadeIn, float delay);
    void removeLights();
    void showFlash();

    cocos2d::CCArray* m_lightStripArray;
    cocos2d::CCLayerColor* m_layerColor;
    bool m_dontFadeOut;
    cocos2d::CCNode* m_mainLayer;
    int m_layerColorZOrder;

};
