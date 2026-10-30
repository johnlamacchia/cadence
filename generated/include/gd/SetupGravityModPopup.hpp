#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupGravityModPopup : public FLAlertLayer {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static SetupGravityModPopup* create(EffectGameObject* object, cocos2d::CCArray* objects, bool gravityTrigger);

    void determineStartValues();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects, bool gravityTrigger);
    void onClose(cocos2d::CCObject* sender);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateItemID();
    void updateTextInputLabel();
    void updateValue();
    void updateValueLabel();

    EffectGameObject* m_gameObject;
    cocos2d::CCArray* m_gameObjects;
    cocos2d::CCLabelBMFont* m_gravityModLabel;
    Slider* m_gravityModSlider;
    float m_gravityMod;
    bool m_gravityTrigger;
    float m_minValue;
    float m_maxValue;
    CCTextInputNode* m_itemIDInput;
    int m_itemID;
    bool m_disableDelegate;

};
