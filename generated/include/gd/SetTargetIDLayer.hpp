#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class SetTargetIDLayer : public SetupTriggerPopup {
public:
    virtual ~SetTargetIDLayer();
    virtual void determineStartValues();
    virtual void valueDidChange(int tag, float value);

    static SetTargetIDLayer* create(EffectGameObject* object, cocos2d::CCArray* objects, std::string title, std::string label, int minimum, int maximum, int objectID);

    bool init(EffectGameObject* object, cocos2d::CCArray* objects, std::string title, std::string label, int minimum, int maximum, int objectID);

    int m_objectID;
    int m_minimumID;
    int m_maximumID;

};
