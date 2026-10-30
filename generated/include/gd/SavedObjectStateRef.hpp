#pragma once

class GameObject;

class SavedObjectStateRef {
public:
    GameObject* m_gameObject;
    double m_positionX;
    double m_positionY;
    float m_rotationXOffset;
    float m_rotationYOffset;
    float m_addToCustomScaleX;
    float m_addToCustomScaleY;
    float m_stepStartX;
    float m_stepStartY;

};
