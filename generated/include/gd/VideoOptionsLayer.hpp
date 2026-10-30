#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class OptionsLayer;
class cocos2d;

class VideoOptionsLayer : public FLAlertLayer {
public:
    virtual ~VideoOptionsLayer();
    virtual bool init();
    virtual void keyBackClicked();

    static VideoOptionsLayer* create();

    CCMenuItemToggler* createToggleButton(std::string label, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position, float scale, float width, bool center, cocos2d::CCArray* nodes);
    void onAdvanced(cocos2d::CCObject* sender);
    void onApply(cocos2d::CCObject* sender);
    void onBorderless(cocos2d::CCObject* sender);
    void onBorderlessFix(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onFullscreen(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onResolutionNext(cocos2d::CCObject* sender);
    void onResolutionPrev(cocos2d::CCObject* sender);
    void onTextureQualityNext(cocos2d::CCObject* sender);
    void onTextureQualityPrev(cocos2d::CCObject* sender);
    void reloadMenu();
    void toggleResolution();
    void updateResolution(int resolution);
    void updateTextureQuality(int quality);

    CCMenuItemSpriteExtra* m_prevResolutionBtn;
    CCMenuItemSpriteExtra* m_nextResolutionBtn;
    cocos2d::CCLabelBMFont* m_windowedResolutionLabel;
    cocos2d::CCLabelBMFont* m_selectedResolutionLabel;
    cocos2d::CCLabelBMFont* m_qualityLabel;
    int m_textureQuality;
    cocos2d::CCArray* m_availableResolutions;
    CCMenuItemToggler* m_borderlessToggle;
    cocos2d::CCLabelBMFont* m_borderlessLabel;
    CCMenuItemToggler* m_fixToggle;
    cocos2d::CCLabelBMFont* m_fixLabel;
    bool m_fullscreen;
    bool m_borderless;
    bool m_fix;
    int m_currentResolution;

};
