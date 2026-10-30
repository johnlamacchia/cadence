#pragma once

class GameObject;
class SFXTriggerGameObject;
class cocos2d;

class SetupReverbPopup : public SetupTriggerPopup {
public:
    virtual ~SetupReverbPopup();
    virtual void onClose(cocos2d::CCObject* sender);

    static SetupReverbPopup* create(SFXTriggerGameObject* object, cocos2d::CCArray* objects);

    SetupReverbPopup();
    bool init(SFXTriggerGameObject* object, cocos2d::CCArray* objects);
    void onPlay(cocos2d::CCObject* sender);
    void onReverb(cocos2d::CCObject* sender);
    void updateReverbLabel();

    cocos2d::CCLabelBMFont* m_reverbLabel;

};
