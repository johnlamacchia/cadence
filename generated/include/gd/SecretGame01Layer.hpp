#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class SecretGame01Layer : public cocos2d::CCLayer {
public:
    virtual void update(float dt);
    virtual bool init();

    static SecretGame01Layer* create();

    void didSelectCorrectObject(CCMenuItemSpriteExtra* button);
    void gameStep01();
    void gameStep02();
    int getCountForDifficulty(int difficulty);
    std::string getFrameForDifficulty(int difficulty);
    int getRowsForDifficulty(int difficulty);
    int getTimeForDifficulty(int difficulty);
    void onSelectButton(cocos2d::CCObject* sender);
    void resetGame();
    void resetGameTimer();
    void scaleOutGame(bool won);
    void showGameOver();
    void showGameWon();

    cocos2d::CCArray* m_gameObjects;
    cocos2d::CCArray* m_requiredObjects;
    cocos2d::CCMenu* m_mainMenu;
    int m_selectedObject;
    int m_gameDifficulty;
    bool m_gameActive;
    float m_timeElapsed;
    float m_gameTime;
    cocos2d::CCSprite* m_timeBarSprite;

};
