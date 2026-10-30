#pragma once

class GameObject;
class ProgressGameObject;
class cocos2d;

class SetupProgressPopup : public SetupTriggerPopup {
public:
    virtual ~SetupProgressPopup();
    virtual void valueDidChange(int, float);

    static SetupProgressPopup* create(ProgressGameObject*, cocos2d::CCArray*);

    bool init(ProgressGameObject*, cocos2d::CCArray*);

};
