#pragma once

class CCMenuItemToggler;
class CCTextInputNode;
class EffectGameObject;
class GameObject;
class cocos2d;

class SetupTouchTogglePopup : public SetupTriggerPopup {
public:
    virtual ~SetupTouchTogglePopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupTouchTogglePopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onControlMode(cocos2d::CCObject* sender);
    void onDualTouchMode(cocos2d::CCObject* sender);
    void onHoldMode(cocos2d::CCObject* sender);
    void onTouchMode(cocos2d::CCObject* sender);
    void updateTargetID();
    void updateTextInputLabel();

    CCTextInputNode* m_groupIDInput;
    int m_groupID;
    bool m_holdMode;
    int m_toggleMode;
    int m_playerMode;
    CCMenuItemToggler* m_toggleOnToggler;
    CCMenuItemToggler* m_toggleOffToggler;
    bool m_dualMode;
    CCMenuItemToggler* m_p1OnlyToggler;
    CCMenuItemToggler* m_p2OnlyToggler;

};
