#pragma once

class CCMenuItemSpriteExtra;
class RateLevelDelegate;
class cocos2d;

class RateLevelLayer : public FLAlertLayer {
public:
    virtual void keyBackClicked();

    static RateLevelLayer* create(int levelID);

    RateLevelLayer();
    ~RateLevelLayer();
    bool init(int levelID);
    void onClose(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);

    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCArray* m_difficulties;
    int m_levelID;
    int m_levelRate;
    RateLevelDelegate* m_delegate;

};
