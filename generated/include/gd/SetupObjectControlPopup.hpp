#pragma once

class GameObject;
class ObjectControlGameObject;
class cocos2d;

class SetupObjectControlPopup : public SetupTriggerPopup {
public:
    virtual ~SetupObjectControlPopup();

    static SetupObjectControlPopup* create(ObjectControlGameObject* object, cocos2d::CCArray* objects);

    bool init(ObjectControlGameObject* object, cocos2d::CCArray* objects);

};
