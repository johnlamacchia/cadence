#pragma once

class cocos2d;

class CreateStartPosPopup : public SetupTriggerPopup {
public:
    virtual ~CreateStartPosPopup();
    virtual bool init();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void updateInputNode(int, float);

    static CreateStartPosPopup* create();

    void onCreate(cocos2d::CCObject* sender);

};
