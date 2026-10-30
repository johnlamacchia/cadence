#pragma once

class CCTextInputNode;
class EffectGameObject;
class GameObject;
class cocos2d;

class SetupEndPopup : public SetupTriggerPopup {
public:
    virtual ~SetupEndPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);

    static SetupEndPopup* create(EffectGameObject* object, cocos2d::CCArray* objects);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects);
    void onLockY(cocos2d::CCObject* sender);
    void onMultiActivate(cocos2d::CCObject* sender);
    void onReversed(cocos2d::CCObject* sender);
    void updateTargetID();
    void updateTextInputLabel();

    CCTextInputNode* m_groupIDInput;
    bool m_reversed;
    bool m_lockY;
    int m_groupID;
    bool m_multiActivate;

};
