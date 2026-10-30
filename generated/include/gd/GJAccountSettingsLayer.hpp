#pragma once

class CCMenuItemToggler;
class cocos2d;

class GJAccountSettingsLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static GJAccountSettingsLayer* create(int accountID);

    GJAccountSettingsLayer();
    ~GJAccountSettingsLayer();
    void closeAccInputNodes(cocos2d::CCArray* nodes);
    CCMenuItemToggler* createToggleButton(std::string label, cocos2d::SEL_MenuHandler selector, bool toggled, cocos2d::CCMenu* menu, cocos2d::CCPoint position, float width, float scale);
    bool init(int accountID);
    void onClose(cocos2d::CCObject* sender);
    void onCommentSetting(cocos2d::CCObject* sender);
    void onFriendRequests(cocos2d::CCObject* sender);
    void onGroupFriends(cocos2d::CCObject* sender);
    void onMessageSetting(cocos2d::CCObject* sender);
    void onSocialLinks(cocos2d::CCObject* sender);
    void onUpdate(cocos2d::CCObject* sender);
    void updateScoreValues();

    int m_accountID;
    int m_messageStatus;
    int m_friendStatus;
    int m_commentHistoryStatus;
    int m_groupStatus;
    int m_unk2ac;
    cocos2d::CCArray* m_inputs;
    std::map<int, std::string> m_links;
    cocos2d::CCArray* m_messageSettings;
    cocos2d::CCArray* m_friendRequestSettings;
    cocos2d::CCArray* m_commentSettings;
    cocos2d::CCArray* m_groupInviteSettings;

};
