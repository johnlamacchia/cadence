#pragma once

class GJGroundLayer;
class PlayerObject;
class cocos2d;

class MenuGameLayer : public cocos2d::CCLayer {
public:
    virtual ~MenuGameLayer();
    virtual void update(float dt);
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();

    static MenuGameLayer* create();

    MenuGameLayer();
    void destroyPlayer();
    cocos2d::ccColor3B getBGColor(int index);
    void resetPlayer();
    void tryJump(float dt);
    void updateColor(float dt);
    void updateColors();

    bool m_videoOptionsOpen;
    float m_deltaCount;
    bool m_isDestroyingPlayer;
    int m_initCount;
    cocos2d::CCPoint m_realBackgroundPosition;
    PlayerObject* m_playerObject;
    cocos2d::CCSprite* m_backgroundSprite;
    GJGroundLayer* m_groundLayer;
    float m_backgroundWrapPosition;

};
