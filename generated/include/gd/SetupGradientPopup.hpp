#pragma once

class GradientTriggerObject;
class cocos2d;

class SetupGradientPopup : public SetupTriggerPopup {
public:
    virtual void determineStartValues();
    virtual void updateToggleItem(int value, bool toggled);
    virtual void valueDidChange(int tag, float value);

    static SetupGradientPopup* create(GradientTriggerObject* object, cocos2d::CCArray* objects);

    SetupGradientPopup();
    ~SetupGradientPopup();
    bool init(GradientTriggerObject* object, cocos2d::CCArray* objects);
    void onBlending(cocos2d::CCObject* sender);
    void onZLayer(cocos2d::CCObject* sender);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateBlending();
    void updateBlendingLabel();
    void updateGradientLabels(bool vertex);
    void updateZLayerButtons();

    cocos2d::CCLabelBMFont* m_blendingLabel;
    int m_blendingMode;
    cocos2d::CCArray* m_layerButtons;
    int m_blendingLayer;
    cocos2d::CCArray* m_gradientLabels;

};
