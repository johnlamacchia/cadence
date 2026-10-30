#pragma once

class cocos2d;

class SetupAudioTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupAudioTriggerPopup();
    virtual void valueDidChange(int tag, float value);

    void addProxVolControls(int page);
    void addTimeControls(int page, float yOffset);
    void onProxMode(cocos2d::CCObject* sender);
    void updateSpecialGroup();

    cocos2d::CCArray* m_proximityButtons;

};
