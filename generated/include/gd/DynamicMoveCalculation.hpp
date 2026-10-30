#pragma once

class CCMoveCNode;
class GameObject;
class cocos2d;

class DynamicMoveCalculation {
public:
    CCMoveCNode* m_moveNode;
    cocos2d::CCPoint m_offset;
    GameObject* m_gameObject;

};
