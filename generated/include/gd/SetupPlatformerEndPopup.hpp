#pragma once

class EndTriggerGameObject;
class GameObject;
class cocos2d;

class SetupPlatformerEndPopup : public SetupTriggerPopup {
public:
    virtual ~SetupPlatformerEndPopup();

    static SetupPlatformerEndPopup* create(EndTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(EndTriggerGameObject* object, cocos2d::CCArray* objects);

};
