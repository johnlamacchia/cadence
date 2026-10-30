#pragma once

class GameObject;
class cocos2d;

class GameObjectPhysics {
public:
    GameObject* m_gameObject;
    cocos2d::CCPoint m_velocity;
    cocos2d::CCPoint m_direction;
    float m_targetAngle;
    float m_turnSpeed;
    int m_unkInt1;
    int m_lastUpdateStep;
    int m_exclusiveStep;

};
