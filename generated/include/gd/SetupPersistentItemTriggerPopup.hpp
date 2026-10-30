#pragma once

class GameObject;
class ItemTriggerGameObject;
class cocos2d;

class SetupPersistentItemTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupPersistentItemTriggerPopup();
    virtual void valueDidChange(int, float);

    static SetupPersistentItemTriggerPopup* create(ItemTriggerGameObject* object, cocos2d::CCArray* objects);

    bool init(ItemTriggerGameObject* object, cocos2d::CCArray* objects);

};
