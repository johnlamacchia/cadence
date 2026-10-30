#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class GameObject;
class RandTriggerGameObject;
class cocos2d;

class SetupRandAdvTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupRandAdvTriggerPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode* node);

    static SetupRandAdvTriggerPopup* create(RandTriggerGameObject* object, cocos2d::CCArray* objects);

    void addChance(int id, int chance);
    void addChanceToObject(RandTriggerGameObject* object, int id, int chance);
    void callRemoveFromGroup(float dt);
    bool init(RandTriggerGameObject* object, cocos2d::CCArray* objects);
    void onAddChance(cocos2d::CCObject* sender);
    void onGroupIDScrollDown(cocos2d::CCObject* sender);
    void onGroupIDScrollUp(cocos2d::CCObject* sender);
    void onRemoveFromGroup(cocos2d::CCObject* sender);
    void removeGroupID(int id);
    void removeGroupIDFromObject(RandTriggerGameObject* object, int id);
    void updateGroupIDButtons();

    bool m_performedAction;
    cocos2d::CCArray* m_groupButtons;
    CCTextInputNode* m_groupIDInput;
    CCTextInputNode* m_chanceInput;
    int m_groupToRemove;
    bool m_removingGroup;
    CCMenuItemSpriteExtra* m_scrollUpButton;
    CCMenuItemSpriteExtra* m_scrollDownButton;
    int m_groupIDScrollIndex;

};
