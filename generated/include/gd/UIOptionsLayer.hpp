#pragma once

class GJUINode;
class OptionsLayer;
class cocos2d;

class UIOptionsLayer : public SetupTriggerPopup {
public:
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual float getValue(int tag);

    static UIOptionsLayer* create(bool dual);

    GJUINode* getNode(int type);
    bool init(bool dual);
    void onReset(cocos2d::CCObject* sender);
    void onSaveLoad(cocos2d::CCObject* sender);
    void toggleUIGroup(int group);

    bool m_dual;
    GJUINode* m_uiNode1;
    GJUINode* m_uiNode2;
    GJUINode* m_uiNode3;
    GJUINode* m_uiNode4;
    cocos2d::CCLabelBMFont* m_nameLabel;
    int m_activeUIGroup;

};
