#pragma once

class CCMenuItemSpriteExtra;
class DialogLayer;
class cocos2d;

class LevelAreaInnerLayer : public cocos2d::CCLayer, public DialogDelegate {
public:
    virtual ~LevelAreaInnerLayer();
    virtual void keyBackClicked();
    virtual void dialogClosed(DialogLayer* layer);
    virtual void onExit();

    static LevelAreaInnerLayer* create(bool returning);
    static cocos2d::CCScene* scene(bool returning);

    LevelAreaInnerLayer();
    bool init(bool returning);
    void onBack(cocos2d::CCObject* sender);
    void onDoor(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNextFloor(cocos2d::CCObject* sender);
    void onOnlineVault(cocos2d::CCObject* sender);
    bool playStep1();
    void showFloor1CompleteDialog();
    void tryResumeTowerMusic();
    void tryShowAd();

    CCMenuItemSpriteExtra* m_nextFloorButton;
    bool m_enteringLevel;
    int m_levelID;
    bool m_exiting;

};
