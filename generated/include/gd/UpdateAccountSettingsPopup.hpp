#pragma once

class CCMenuItemSpriteExtra;
class GJAccountSettingsLayer;
class LoadingCircle;
class TextArea;
class cocos2d;

class UpdateAccountSettingsPopup : public FLAlertLayer, public GJAccountSettingsDelegate {
public:
    virtual void keyBackClicked();
    virtual void updateSettingsFinished();
    virtual void updateSettingsFailed();

    static UpdateAccountSettingsPopup* create(GJAccountSettingsLayer*, int, int, int, int, int, std::string, std::string, std::string, std::string, std::string, std::string, std::string);

    UpdateAccountSettingsPopup();
    ~UpdateAccountSettingsPopup();
    bool init(GJAccountSettingsLayer*, int, int, int, int, int, std::string, std::string, std::string, std::string, std::string, std::string, std::string);
    void onClose(cocos2d::CCObject* sender);

    bool m_updateSuccess;
    GJAccountSettingsLayer* m_settingsLayer;
    TextArea* m_textArea;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_closeButton;

};
