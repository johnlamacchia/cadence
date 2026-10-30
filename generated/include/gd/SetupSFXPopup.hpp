#pragma once

class CustomSFXWidget;
class GameObject;
class SFXBrowser;
class SFXInfoObject;
class SFXTriggerGameObject;
class cocos2d;

class SetupSFXPopup : public SetupAudioTriggerPopup, public CustomSFXDelegate, public SFXBrowserDelegate {
public:
    virtual ~SetupSFXPopup();
    virtual void pageChanged();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onPlusButton(cocos2d::CCObject* sender);
    virtual void valueDidChange(int tag, float value);
    virtual void sfxObjectSelected(SFXInfoObject* object);
    virtual int getActiveSFXID();
    virtual bool overridePlaySFX(SFXInfoObject* object);
    virtual void sfxBrowserClosed(SFXBrowser* browser);
    virtual int getActiveCustomSFXID();

    static SetupSFXPopup* create(SFXTriggerGameObject*, cocos2d::CCArray*, int);

    void createSFXWidget();
    bool init(SFXTriggerGameObject*, cocos2d::CCArray*, int);
    void onBrowseSFX(cocos2d::CCObject* sender);
    void updateLength();

    int m_sfxID;
    CustomSFXWidget* m_sfxWidget;
    int m_sfxLength;

};
