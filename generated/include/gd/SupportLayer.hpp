#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class UploadActionPopup;
class cocos2d;

class SupportLayer : public GJDropDownLayer, public FLAlertLayerProtocol, public UploadActionDelegate, public UploadPopupDelegate {
public:
    virtual void customSetup();
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static SupportLayer* create();

    SupportLayer();
    ~SupportLayer();
    CCMenuItemToggler* createToggleButton(std::string text, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position, cocos2d::CCArray* nodes);
    void exitLayer();
    void onCocos2d(cocos2d::CCObject* sender);
    void onEmail(cocos2d::CCObject* sender);
    void onGetReward(cocos2d::CCObject* sender);
    void onLinks(cocos2d::CCObject* sender);
    void onLowDetail(cocos2d::CCObject* sender);
    void onPrivacy(cocos2d::CCObject* sender);
    void onRequestAccess(cocos2d::CCObject* sender);
    void onRobTop(cocos2d::CCObject* sender);
    void onSFX(cocos2d::CCObject* sender);
    void onTOS(cocos2d::CCObject* sender);
    void sendSupportMail();

    UploadActionPopup* m_uploadPopup;

};
