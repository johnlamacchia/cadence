#pragma once

class GameObject;
class cocos2d;

class GameObjectEditorState {
public:
    void loadValues(GameObject* obj);

    cocos2d::CCPoint m_position;
    float m_scaleX;
    float m_scaleY;
    float m_rotationX;
    float m_rotationY;

};
