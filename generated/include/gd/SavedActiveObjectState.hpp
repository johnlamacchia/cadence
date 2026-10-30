#pragma once

class GameObject;

class SavedActiveObjectState {
public:
    GameObject* m_gameObject;
    bool m_activatedByPlayer1;
    bool m_activatedByPlayer2;

};
