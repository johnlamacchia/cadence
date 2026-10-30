#pragma once

class CCMenuItemSpriteExtra;
class GameObject;
class SequenceTriggerGameObject;
class cocos2d;

class SetupSequenceTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupSequenceTriggerPopup();
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupSequenceTriggerPopup* create(SequenceTriggerGameObject* object);

    bool init(SequenceTriggerGameObject* object);
    void onAddChance(cocos2d::CCObject* sender);
    void onChangeOrder(cocos2d::CCObject* sender);
    void onDeleteSelected(cocos2d::CCObject* sender);
    void onSelect(cocos2d::CCObject* sender);
    void updateGroupIDButtons();

    CCMenuItemSpriteExtra* m_selectedButton;
    bool m_unk3b0;
    bool m_unk3b1;
    cocos2d::CCArray* m_groupButtons;
    int m_unk3c0;
    bool m_unk3c4;

};
