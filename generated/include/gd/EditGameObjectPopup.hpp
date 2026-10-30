#pragma once

class EffectGameObject;
class GameObject;
class cocos2d;

class EditGameObjectPopup : public SetupTriggerPopup {
public:
    virtual ~EditGameObjectPopup();

    static EditGameObjectPopup* create(EffectGameObject* object, cocos2d::CCArray* objects, bool platformer);

    EditGameObjectPopup();
    bool init(EffectGameObject* object, cocos2d::CCArray* objects, bool platformer);

};
