#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class Slider;
class cocos2d;

class SetupRandTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupRandTriggerPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupRandTriggerPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void sliderChanged(cocos2d::CCObject* sender);
    void updateChance();
    void updateChanceLabel(bool decimals);
    void updateTargetID();
    void updateTargetID2();
    void updateTextInputLabel();
    void updateTextInputLabel2();

    CCTextInputNode* m_chanceInput;
    Slider* m_chanceSlider;
    float m_chancePercent;
    CCTextInputNode* m_groupID1Input;
    CCTextInputNode* m_groupID2Input;
    int m_groupID1;
    int m_groupID2;

};
