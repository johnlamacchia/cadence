#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class GJGameLevel;
class cocos2d;

class ShareLevelLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static ShareLevelLayer* create(GJGameLevel* level);

    ShareLevelLayer();
    ~ShareLevelLayer();
    CCMenuItemSpriteExtra* getStarsButton(int btnID, cocos2d::SEL_MenuHandler selector, cocos2d::CCMenu* menu, float scale);
    bool init(GJGameLevel* level);
    void onClose(cocos2d::CCObject* sender);
    void onGuidelines(cocos2d::CCObject* sender);
    void onNotifyFollowers(cocos2d::CCObject* sender);
    void onNotifyFriends(cocos2d::CCObject* sender);
    void onSettings(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);
    void /* TodoReturn */ setupNotifyToggles(cocos2d::CCPoint, float, float);
    void setupStars();
    void /* TodoReturn */ updateNotifyToggles();

    GJGameLevel* m_level;
    int m_starsRequested;
    cocos2d::CCArray* m_starButtons;
    cocos2d::CCSprite* m_difficultySprite;
    cocos2d::CCLabelBMFont* m_notifyLabel;
    cocos2d::CCSprite* m_notifySprite;
    CCMenuItemToggler* m_friendsToggler;
    CCMenuItemToggler* m_followersToggler;
    cocos2d::CCSprite* m_friendsSprite;
    cocos2d::CCSprite* m_followersSprite;
    cocos2d::CCLabelBMFont* m_friendsLabel;
    cocos2d::CCLabelBMFont* m_followersLabel;

};
