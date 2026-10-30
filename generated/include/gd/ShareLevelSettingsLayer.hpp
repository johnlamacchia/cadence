#pragma once

class CCMenuItemToggler;
class GJGameLevel;
class LevelSettingsLayer;
class ShareLevelLayer;
class cocos2d;

class ShareLevelSettingsLayer : public FLAlertLayer, public NumberInputDelegate {
public:
    virtual ~ShareLevelSettingsLayer();
    virtual void keyBackClicked();

    static ShareLevelSettingsLayer* create(GJGameLevel*, ShareLevelLayer*);

    ShareLevelSettingsLayer();
    bool init(GJGameLevel*, ShareLevelLayer*);
    void onClose(cocos2d::CCObject* sender);
    void onUnlisted(cocos2d::CCObject* sender);
    void onUnlistedFriendsOnly(cocos2d::CCObject* sender);
    void updateSettingsState();

    cocos2d::CCLabelBMFont* m_passwordLabel;
    GJGameLevel* m_level;
    CCMenuItemToggler* m_friendsOnlyToggler;
    cocos2d::CCLabelBMFont* m_friendsOnlyLabel;
    ShareLevelLayer* m_shareLevelLayer;

};
