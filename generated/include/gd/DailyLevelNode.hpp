#pragma once

class CCMenuItemSpriteExtra;
class DailyLevelPage;
class FLAlertLayer;
class GJGameLevel;
class LevelPage;
class cocos2d;

class DailyLevelNode : public cocos2d::CCNode, public FLAlertLayerProtocol {
public:
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static DailyLevelNode* create(GJGameLevel* level, DailyLevelPage* page, bool isNew);

    DailyLevelNode();
    ~DailyLevelNode();
    bool init(GJGameLevel* level, DailyLevelPage* page, bool isNew);
    void onClaimReward(cocos2d::CCObject* sender);
    void onSkipLevel(cocos2d::CCObject* sender);
    void showSkipButton();
    void updateTimeLabel(std::string text);

    GJGameLevel* m_level;
    DailyLevelPage* m_page;
    cocos2d::CCLabelBMFont* m_timeLabel;
    cocos2d::CCPoint m_unkPoint;
    CCMenuItemSpriteExtra* m_skipButton;
    bool m_unkBool;
    bool m_needsDownloading;

};
