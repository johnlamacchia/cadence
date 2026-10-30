#pragma once

class GJBaseGameLayer;
class OptionsLayer;
class cocos2d;

class GameOptionsLayer : public GJOptionsLayer {
public:
    virtual void setupOptions();
    virtual void didToggle(int tag);

    static GameOptionsLayer* create(GJBaseGameLayer* baseGameLayer);

    GameOptionsLayer();
    bool init(GJBaseGameLayer* baseGameLayer);
    void onPracticeMusicSync(cocos2d::CCObject* sender);
    void onUIOptions(cocos2d::CCObject* sender);
    void onUIPOptions(cocos2d::CCObject* sender);
    void showPracticeMusicSyncUnlockInfo();

    GJBaseGameLayer* m_baseGameLayer;
    int m_practiceDialogIndex;

};
