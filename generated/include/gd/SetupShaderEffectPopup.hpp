#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetupShaderEffectPopup : public SetupTriggerPopup {
public:
    virtual void toggleGroup(int group, bool visible);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);

    static SetupShaderEffectPopup* create(EffectGameObject* object, cocos2d::CCArray* objects, int id);

    SetupShaderEffectPopup();
    ~SetupShaderEffectPopup();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects, int id);
    void onResetColors(cocos2d::CCObject* sender);
    void onZLayer(cocos2d::CCObject* sender);
    void setupBulge();
    void setupChromatic();
    void setupChromaticGlitch();
    void setupColorChange();
    void setupGlitch();
    void setupGrayscale();
    void setupHueShift();
    void setupInvertColor();
    void setupLensCircle();
    void setupMotionBlur();
    void setupPinch();
    void setupPixelate();
    void setupRadialBlur();
    void setupSepia();
    void setupShaderTrigger();
    void setupShockLine();
    void setupShockWave();
    void setupSplitScreen();
    void updateZLayerButtons();
    std::string zLayerToString(int zLayer);

    cocos2d::CCArray* m_zLayerSprites;
    int m_zLayerMin;
    int m_zLayerMax;
    bool m_changeMin;
    int m_objectID;

};
