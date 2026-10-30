#pragma once

class AdvancedFollowTriggerObject;
class SelectPremadeLayer;
class cocos2d;

class SetupAdvFollowPopup : public SetupTriggerPopup, public SelectPremadeDelegate {
public:
    virtual ~SetupAdvFollowPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void updateDefaultTriggerValues();
    virtual void valueDidChange(int tag, float value);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);
    virtual void selectPremadeClosed(SelectPremadeLayer* layer, int type);

    static SetupAdvFollowPopup* create(AdvancedFollowTriggerObject* object, cocos2d::CCArray* objects);

    bool init(AdvancedFollowTriggerObject* object, cocos2d::CCArray* objects);
    void onMode(cocos2d::CCObject* sender);
    void onPremade(cocos2d::CCObject* sender);
    void updateMode(int mode);

};
