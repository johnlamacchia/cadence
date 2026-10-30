#pragma once

class GameObject;
class cocos2d;

class TraceDrawObject {
public:
    TraceDrawObject();
    TraceDrawObject(int type, GameObject* fromObject, GameObject* toObject, cocos2d::ccColor4F color);
    TraceDrawObject(int groupID, cocos2d::ccColor4F color);

    int m_type;
    GameObject* m_fromObject;
    GameObject* m_toObject;
    int m_groupID;
    cocos2d::ccColor4F m_color;

};
