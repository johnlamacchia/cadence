#pragma once

class DialogLayer;
class GJGameLevel;
class GameObject;
class cocos2d;

class LevelPage : public cocos2d::CCLayer, public DialogDelegate {
public:
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void dialogClosed(DialogLayer* layer);

    static LevelPage* create(GJGameLevel* level);

    LevelPage();
    ~LevelPage();
    void addSecretCoin();
    void addSecretDoor();
    bool init(GJGameLevel* level);
    void onDeathTracker(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onMoreGames(cocos2d::CCObject* sender);
    void onPlay(cocos2d::CCObject* sender);
    void onSecretDoor(cocos2d::CCObject* sender);
    void onTheTower(cocos2d::CCObject* sender);
    void playCoinEffect();
    void playStep2();
    void playStep3();
    void updateDynamicPage(GJGameLevel* level);

    bool m_isBusy;
    GJGameLevel* m_level;
    cocos2d::CCMenu* m_levelMenu;
    cocos2d::extension::CCScale9Sprite* m_levelDisplay;
    cocos2d::CCLabelBMFont* m_normalProgressLabel;
    cocos2d::CCLabelBMFont* m_practiceProgressLabel;
    cocos2d::CCLabelBMFont* m_nameLabel;
    cocos2d::CCLabelBMFont* m_starsLabel;
    float m_progressWidth;
    cocos2d::CCSprite* m_normalProgressBar;
    cocos2d::CCSprite* m_practiceProgressBar;
    cocos2d::CCSprite* m_difficultySprite;
    cocos2d::CCSprite* m_starsSprite;
    cocos2d::CCSize m_levelDisplaySize;
    cocos2d::CCArray* m_coins;
    cocos2d::CCArray* m_dynamicObjects;
    cocos2d::CCArray* m_levelObjects;
    cocos2d::CCArray* m_progressObjects;
    GameObject* m_coinObject;
    cocos2d::CCSprite* m_secretDoor;

};
