#pragma once

class cocos2d;

class DemonInfoPopup : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static DemonInfoPopup* create(int easyClassic, int mediumClassic, int hardClassic, int insaneClassic, int extremeClassic, int easyPlatformer, int mediumPlatformer, int hardPlatformer, int insanePlatformer, int extremePlatformer, int weekly, int gauntlet);
    static DemonInfoPopup* createFromString(std::string str);

    ~DemonInfoPopup();
    bool init(int easyClassic, int mediumClassic, int hardClassic, int insaneClassic, int extremeClassic, int easyPlatformer, int mediumPlatformer, int hardPlatformer, int insanePlatformer, int extremePlatformer, int weekly, int gauntlet);
    void onClose(cocos2d::CCObject* sender);

};
