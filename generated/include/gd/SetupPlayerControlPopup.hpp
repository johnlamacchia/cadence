#pragma once

class GameObject;
class PlayerControlGameObject;
class cocos2d;

class SetupPlayerControlPopup : public SetupTriggerPopup {
public:
    virtual ~SetupPlayerControlPopup();

    static SetupPlayerControlPopup* create(PlayerControlGameObject* object, cocos2d::CCArray* objects);

    bool init(PlayerControlGameObject* object, cocos2d::CCArray* objects);

};
