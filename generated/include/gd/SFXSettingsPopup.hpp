#pragma once

class SFXBrowser;
class cocos2d;

class SFXSettingsPopup : public GJOptionsLayer {
public:
    virtual ~SFXSettingsPopup();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void setupOptions();
    virtual void didToggleGV(std::string);

    static SFXSettingsPopup* create(SFXBrowser*);

    bool init(SFXBrowser*);

};
