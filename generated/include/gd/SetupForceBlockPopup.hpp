#pragma once

class ForceBlockGameObject;
class GameObject;
class cocos2d;

class SetupForceBlockPopup : public SetupTriggerPopup {
public:
    virtual ~SetupForceBlockPopup();
    virtual void valueDidChange(int tag, float value);

    static SetupForceBlockPopup* create(ForceBlockGameObject* object, cocos2d::CCArray* objects);

    bool init(ForceBlockGameObject* object, cocos2d::CCArray* objects);

};
