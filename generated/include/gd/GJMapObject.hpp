#pragma once

class cocos2d;

class GJMapObject : public cocos2d::CCNode {
public:
    static GJMapObject* create(cocos2d::CCNode* parent);
    static GJMapObject* createMonster(int objectID);

    bool init(cocos2d::CCNode* parent);
    void monsterIdle();
    void monsterJump();
    void moveFinished();
    void moveMonster();
    void moveMonster(cocos2d::CCPoint position, float offset);
    void playerJump();
    void setObjectOrigin(cocos2d::CCPoint position, float offset);
    void startMonsterJumpLoop();
    void startPlayerJumpLoop();
    void touchMonster();
    void updateShadow(float scaleX, float scaleY, cocos2d::CCPoint position);

    float m_jumpStopTime;
    cocos2d::CCNode* m_mainNode;
    cocos2d::CCSprite* m_shadowSprite;
    cocos2d::CCPoint m_objectOrigin;
    float m_objectOffset;
    bool m_monsterIdle;
    bool m_monsterMoving;
    float m_moveStopTime;
    int m_monstersTouched;

};
