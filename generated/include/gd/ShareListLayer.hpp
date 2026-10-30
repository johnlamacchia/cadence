#pragma once

class CCMenuItemToggler;
class GJLevelList;
class cocos2d;

class ShareListLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static ShareListLayer* create(GJLevelList* list);

    ShareListLayer();
    ~ShareListLayer();
    bool init(GJLevelList* list);
    void onClose(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNotifyFollowers(cocos2d::CCObject* sender);
    void onNotifyFriends(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void onUnlisted(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateNotifyToggles();
    void updateUnlistedF();

    GJLevelList* m_list;
    CCMenuItemToggler* m_friendsOnlyToggler;
    cocos2d::CCLabelBMFont* m_friendsOnlyLabel;
    cocos2d::CCLabelBMFont* m_notifyLabel;
    cocos2d::CCSprite* m_notifySprite;
    CCMenuItemToggler* m_friendsToggler;
    CCMenuItemToggler* m_followersToggler;
    cocos2d::CCSprite* m_friendsSprite;
    cocos2d::CCSprite* m_followersSprite;
    cocos2d::CCLabelBMFont* m_friendsLabel;
    cocos2d::CCLabelBMFont* m_followersLabel;

};
