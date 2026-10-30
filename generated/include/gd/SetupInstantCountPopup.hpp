#pragma once

class CCMenuItemToggler;
class CCTextInputNode;
class CountTriggerGameObject;
class GameObject;
class cocos2d;

class SetupInstantCountPopup : public SetupTriggerPopup {
public:
    virtual ~SetupInstantCountPopup();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupInstantCountPopup* create(CountTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(CountTriggerGameObject* object, cocos2d::CCArray* objects);
    void onCountType(cocos2d::CCObject* sender);
    void onEnableGroup(cocos2d::CCObject* sender);
    void onTargetCountArrow(cocos2d::CCObject* sender);
    void updateCountTextInputLabel();
    void updateItemID();
    void updateItemIDInputLabel();
    void updateTargetCount();
    void updateTargetID();
    void updateTargetIDInputLabel();

    CCTextInputNode* m_itemIDInput;
    CCTextInputNode* m_targetIDInput;
    CCTextInputNode* m_targetCountInput;
    int m_itemID;
    int m_targetID;
    int m_targetCount;
    bool m_activateGroup;
    int m_mode;
    CCMenuItemToggler* m_equalsToggler;
    CCMenuItemToggler* m_largerToggler;
    CCMenuItemToggler* m_smallerToggler;

};
