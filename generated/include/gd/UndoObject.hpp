#pragma once

class GJTransformState;
class GameObject;
class GameObjectCopy;
class cocos2d;

class UndoObject : public cocos2d::CCObject {
public:
    static UndoObject* create(GameObject* object, UndoCommand command);
    static UndoObject* createWithArray(cocos2d::CCArray* array, UndoCommand command);
    static UndoObject* createWithTransformObjects(cocos2d::CCArray* objects, UndoCommand command);

    UndoObject();
    ~UndoObject();
    bool init(GameObject* object, UndoCommand command);
    bool init(cocos2d::CCArray* array, UndoCommand command);
    bool initWithTransformObjects(cocos2d::CCArray* objects, UndoCommand command);
    void setObjects(cocos2d::CCArray* objects);

    GameObjectCopy* m_objectCopy;
    UndoCommand m_command;
    cocos2d::CCArray* m_objects;
    bool m_redo;
    bool m_undoTransform;
    GJTransformState m_transformState;
    bool m_unk8c;
    int m_unk90;

};
