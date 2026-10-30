#pragma once

class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupCameraModePopup : public SetupTriggerPopup {
public:
    virtual ~SetupCameraModePopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupCameraModePopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onEditCameraSettings(cocos2d::CCObject* sender);
    void onUnboundMode(cocos2d::CCObject* sender);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateCameraEasing();
    void updateCameraEasingLabel();
    void updateCameraPadding();
    void updateCameraPaddingLabel();
    void updateItemVisibility();

    cocos2d::CCArray* m_freeModeObjects;
    cocos2d::CCArray* m_cameraSettingsObjects;
    Slider* m_cameraEasingSlider;
    cocos2d::CCLabelBMFont* m_cameraEasingLabel;
    int m_cameraEasing;
    Slider* m_cameraPaddingSlider;
    cocos2d::CCLabelBMFont* m_cameraPaddingLabel;
    float m_cameraPadding;
    bool m_freeMode;
    bool m_cameraSettingsEnabled;
    bool m_cameraModeObjects;

};
