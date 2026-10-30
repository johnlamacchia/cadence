#pragma once

class ColorSelectDelegate;
class ConfigureHSVWidget;
class GameObject;
class cocos2d;

class HSVLiveOverlay : public FLAlertLayer, public HSVWidgetDelegate {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void hsvChanged(ConfigureHSVWidget* widget);

    static HSVLiveOverlay* create(GameObject*, cocos2d::CCArray*, cocos2d::ccColor3B, cocos2d::ccColor3B);

    HSVLiveOverlay();
    ~HSVLiveOverlay();
    void closeColorSelect(cocos2d::CCObject* sender);
    void createHSVWidget(int tab);
    void determineStartValues();
    bool init(GameObject*, cocos2d::CCArray*, cocos2d::ccColor3B, cocos2d::ccColor3B);
    void onSelectTab(cocos2d::CCObject* sender);
    void toggleControls(bool visible);

    GameObject* m_object;
    cocos2d::CCArray* m_objects;
    cocos2d::CCArray* m_controls;
    cocos2d::CCArray* m_unkArray;
    ColorSelectDelegate* m_delegate;
    int m_activeTab;
    ConfigureHSVWidget* m_widget;
    bool m_unkBool1;
    bool m_unkBool2;
    bool m_unkBool3;
    cocos2d::ccColor3B m_startColor1;
    cocos2d::ccColor3B m_startColor2;

};
