#pragma once

class CCMenuItemSpriteExtra;
class FLAlertLayer;
class GJDifficultySprite;
class GJGameLevel;
class LoadingCircle;
class RateLevelLayer;
class UploadActionPopup;
class cocos2d;

class GJRateLevelLayer : public SetupTriggerPopup, public UploadPopupDelegate, public UploadActionDelegate, public LevelRateInfoDelegate, public FLAlertLayerProtocol {
public:
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomButton(cocos2d::CCObject* sender);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void rateInfoFinished(int id, std::string info);
    virtual void rateInfoFailed(int id, int response);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    static GJRateLevelLayer* create(GJGameLevel* level);

    GJRateLevelLayer();
    ~GJRateLevelLayer();
    void createStatPillar(int width, int height, int count, cocos2d::ccColor3B innerColor, cocos2d::ccColor3B outerColor, cocos2d::CCPoint position, std::string text);
    CCMenuItemSpriteExtra* getStarsButton(int stars, cocos2d::SEL_MenuHandler selector, cocos2d::CCMenu* menu, float scale);
    bool init(GJGameLevel* level);
    void onFeature(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void onToggleCoins(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);
    void setupRateInfo(std::string info);

    GJGameLevel* m_level;
    bool m_uploadFinished;
    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCArray* m_starButtons;
    int m_stars;
    bool m_coinsToggled;
    cocos2d::CCSprite* m_coinSprite;
    UploadActionPopup* m_uploadPopup;
    GJDifficultySprite* m_difficultySprite;
    GJFeatureState m_featureState;
    LoadingCircle* m_loadingCircle;
    cocos2d::CCPoint m_center;

};
