#pragma once

class GameObject;
class cocos2d;

class GameObjectCopy : public cocos2d::CCObject {
public:
    virtual ~GameObjectCopy();

    static GameObjectCopy* create(GameObject* object);

    bool init(GameObject* object);
    void resetObject();

    GameObject* m_object;
    cocos2d::CCPoint m_position;
    float m_rotationX;
    float m_rotationY;
    bool m_isFlipX;
    bool m_isFlipY;
    float m_customScaleX;
    float m_customScaleY;

};
