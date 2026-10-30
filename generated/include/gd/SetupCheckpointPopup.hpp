#pragma once

class CheckpointGameObject;
class GameObject;
class cocos2d;

class SetupCheckpointPopup : public SetupTriggerPopup {
public:
    virtual ~SetupCheckpointPopup();

    static SetupCheckpointPopup* create(CheckpointGameObject* object, cocos2d::CCArray* objects);

    SetupCheckpointPopup();
    bool init(CheckpointGameObject* object, cocos2d::CCArray* objects);

};
