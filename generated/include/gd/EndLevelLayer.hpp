#pragma once

class PlayLayer;
class cocos2d;

class EndLevelLayer : public GJDropDownLayer {
public:
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void customSetup();
    virtual void showLayer(bool instant);
    virtual void enterAnimFinished();
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);

    static EndLevelLayer* create(PlayLayer* playLayer);

    EndLevelLayer();
    ~EndLevelLayer();
    void coinEnterFinished(cocos2d::CCPoint position);
    void coinEnterFinishedO(cocos2d::CCObject* sender);
    void currencyEnterFinished();
    void diamondEnterFinished();
    std::string getCoinString();
    const char* getEndText();
    void goEdit();
    bool init(PlayLayer* playLayer);
    void onDeathTracker(cocos2d::CCObject* sender);
    void onEdit(cocos2d::CCObject* sender);
    void onEveryplay(cocos2d::CCObject* sender);
    void onHideLayer(cocos2d::CCObject* sender);
    void onLevelLeaderboard(cocos2d::CCObject* sender);
    void onMenu(cocos2d::CCObject* sender);
    void onReplay(cocos2d::CCObject* sender);
    void onRestartCheckpoint(cocos2d::CCObject* sender);
    void playCoinEffect(float duration);
    void playCurrencyEffect(float duration);
    void playDiamondEffect(float duration);
    void playEndEffect();
    void playStarEffect(float duration);
    void starEnterFinished();
    void tryShowBanner(float dt);

    PlayLayer* m_playLayer;
    bool m_notLocal;
    bool m_coinsVerified;
    bool m_unknown3;
    bool m_exiting;
    bool m_animateCoins;
    bool m_endEffectPlayed;
    cocos2d::CCMenu* m_sideMenu;
    cocos2d::CCArray* m_coinsToAnimate;
    cocos2d::CCPoint m_starsPosition;
    cocos2d::CCPoint m_orbsPosition;
    cocos2d::CCPoint m_diamondsPosition;
    int m_orbs;
    int m_stars;
    int m_moons;
    int m_diamonds;
    bool m_secretKey;
    bool m_hidden;

};
