#pragma once

class GameObject;
class TransformTriggerGameObject;
class cocos2d;

class SetupTransformPopup : public SetupTriggerPopup {
public:
    virtual ~SetupTransformPopup();

    static SetupTransformPopup* create(TransformTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(TransformTriggerGameObject* object, cocos2d::CCArray* objects);

};
