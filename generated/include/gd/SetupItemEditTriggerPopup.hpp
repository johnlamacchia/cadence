#pragma once

class CCMenuItemSpriteExtra;
class GameObject;
class ItemTriggerGameObject;
class cocos2d;

class SetupItemEditTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupItemEditTriggerPopup();
    virtual void valueDidChange(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupItemEditTriggerPopup* create(ItemTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(ItemTriggerGameObject* object, cocos2d::CCArray* objects);
    void onOpButton(cocos2d::CCObject* sender);
    void updateFormulaLabel();
    void updateOpButton(CCMenuItemSpriteExtra* button, int key, int objectID);

    cocos2d::CCLabelBMFont* m_formulaLabel;

};
