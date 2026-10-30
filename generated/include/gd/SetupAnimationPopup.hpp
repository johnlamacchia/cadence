#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class cocos2d;

class SetupAnimationPopup : public SetupTriggerPopup {
public:
    virtual ~SetupAnimationPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupAnimationPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onAnimationIDArrow(cocos2d::CCObject* sender);
    void updateAnimationID();
    void updateAnimationTextInputLabel();
    void updateTargetID();
    void updateTextInputLabel();

    CCTextInputNode* m_targetIDInput;
    CCTextInputNode* m_animationIDInput;
    int m_targetID;
    int m_animationID;

};
