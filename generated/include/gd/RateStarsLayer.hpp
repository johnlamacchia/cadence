#pragma once

class CCMenuItemSpriteExtra;
class GJDifficultySprite;
class RateLevelDelegate;
class UploadActionPopup;
class cocos2d;

class RateStarsLayer : public FLAlertLayer, public UploadPopupDelegate, public UploadActionDelegate {
public:
    virtual ~RateStarsLayer();
    virtual void keyBackClicked();
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);

    static RateStarsLayer* create(int levelID, bool platformer, bool moderator);

    CCMenuItemSpriteExtra* getStarsButton(int stars, cocos2d::SEL_MenuHandler selector, cocos2d::CCMenu* menu, float scale);
    bool init(int levelID, bool platformer, bool moderator);
    void onClose(cocos2d::CCObject* sender);
    void onFeature(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void onToggleCoins(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);

    bool m_uploadFinished;
    bool m_moderator;
    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCArray* m_buttons;
    int m_levelID;
    int m_starsRate;
    bool m_coinsToggled;
    cocos2d::CCSprite* m_coinSprite;
    UploadActionPopup* m_popup;
    GJDifficultySprite* m_difficultySprite;
    int m_featureState;
    RateLevelDelegate* m_delegate;

};
