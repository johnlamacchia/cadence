#pragma once

class TouchControlEditLayer;
class cocos2d;

class ControlSlotsPopup : public SetupTriggerPopup {
public:
    virtual ~ControlSlotsPopup();
    virtual void onClose(cocos2d::CCObject* sender);

    static ControlSlotsPopup* create(TouchControlEditLayer*, bool);

    bool init(TouchControlEditLayer*, bool);
    void onLoad(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);

};
