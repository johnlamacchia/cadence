#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class FollowRewardPage : public FLAlertLayer, public FLAlertLayerProtocol, public GameRateDelegate, public RewardedVideoDelegate {
public:
    virtual ~FollowRewardPage();
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void rewardedVideoFinished();
    virtual void updateRate();

    static FollowRewardPage* create();

    void onClose(cocos2d::CCObject* sender);
    void onRewardedVideo(cocos2d::CCObject* sender);
    void onSpecialItem(cocos2d::CCObject* sender);
    void switchToOpenedState(CCMenuItemSpriteExtra* button);

    cocos2d::CCArray* m_chests;
    bool m_videoPlaying;

};
