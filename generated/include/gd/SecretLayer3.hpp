#pragma once

class CCMenuItemSpriteExtra;
class DialogLayer;
class DungeonBarsSprite;
class SecretLayer;
class cocos2d;

class SecretLayer3 : public cocos2d::CCLayer, public DialogDelegate {
public:
    virtual bool init();
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void dialogClosed(DialogLayer* layer);

    static SecretLayer3* create();
    static cocos2d::CCScene* scene();

    SecretLayer3();
    ~SecretLayer3();
    void animateEyes();
    void firstInteractionStep1();
    void firstInteractionStep2();
    void firstInteractionStep3();
    void firstInteractionStep4();
    void onBack(cocos2d::CCObject* sender);
    void onChest01(cocos2d::CCObject* sender);
    void onChest02(cocos2d::CCObject* sender);
    void onClick(cocos2d::CCObject* sender);
    void revealStep1();
    void revealStep2();
    void revealStep3();
    void revealStep4();
    void revealStep5();
    void showUnlockDialog();

    DungeonBarsSprite* m_dungeonBars;
    bool m_lockInput;
    cocos2d::CCArray* m_locksArray;
    cocos2d::CCSprite* m_demonEyes;
    cocos2d::CCSprite* m_demonBody;
    CCMenuItemSpriteExtra* m_secretChest;

};
